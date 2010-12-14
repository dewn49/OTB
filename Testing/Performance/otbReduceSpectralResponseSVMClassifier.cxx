/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbAtmosphericCorrectionsReduceSpectralResponse.h"
#include "otbSatelliteRSR.h"
#include "otbSpectralResponse.h"

#include "otbSVMSampleListModelEstimator.h"
#include "otbSVMClassifier.h"
#include "itkListSample.h"
#include "base/ossimDirectoryTree.h"
#include "base/ossimDirectory.h"
#include "otbConfusionMatrixCalculator.h"


/*
./bin/otbReduceSpectralResponseSVMClassifier ../data/BD-CNES/jpl/nicolet/minerals/borate/txt/ ../data/BD-CNES/jpl/nicolet/minerals/carbonate/txt/ ../data/BD-CNES/jpl/nicolet/minerals/cyclosilicate/txt/ ../data/BD-CNES/jpl/nicolet/manmade/txt/ ../data/BD-CNES/jpl/nicolet/soils/txt/ ../hg/SimuPerformances/Data/Rsr/SPOT5/HRG1/rep6S.dat 4
*/

int otbReduceSpectralResponseSVMClassifier(int argc, char * argv[])
{

   
      
   typedef otb::SpectralResponse< double,double>  ResponseType;
   typedef ResponseType::Pointer  ResponsePointerType;
  
   typedef otb::SatelliteRSR< double,double>  SatRSRType;
   typedef SatRSRType::Pointer  SatRSRPointerType;

   typedef otb::ReduceSpectralResponse < ResponseType,SatRSRType>  ReduceResponseType;
   typedef ReduceResponseType::Pointer  ReduceResponseTypePointerType;
  
   typedef itk::VariableLengthVector<double> SampleType;
   typedef itk::Statistics::ListSample<SampleType>         SampleListType;
   typedef itk::FixedArray<unsigned long, 1>               TrainingSampleType;
   typedef itk::Statistics::ListSample<TrainingSampleType> TrainingSampleListType;
  
   typedef otb::SVMSampleListModelEstimator<SampleListType,TrainingSampleListType> SVMModelEstimatorType;
   typedef otb::SVMClassifier<SampleListType,unsigned long> SVMClassifierType;
   typedef SVMClassifierType::OutputType ClassifierOutputType;
  
  typedef otb::ConfusionMatrixCalculator<TrainingSampleListType,TrainingSampleListType> ConfusionMatrixCalculatorType;
      
   if ( argc!= 9 )
   {
      std::cout << argv[0] << std::endl << "\t" << "<dir_spectres_class1>" << "\t" << "<dir_spectres_class2>" << "\t" << "<dir_spectres_class3>" << "\t" << "<dir_spectres_class4>" << "\t" << "<dir_spectres_class5>"  << "\t" << "<Gabarit_SAT_fileSRname>" << "\t" << "<nbBands>" << "\t" << "<percentage_validation_files>" << std::endl ;
      return EXIT_FAILURE;
   }

   
   //Instantiation
  
   //Load fileSR into vector
   std::vector<ossimFilename> dirSR;
   dirSR.push_back(argv[1]);
   dirSR.push_back(argv[2]);
   dirSR.push_back(argv[3]);
   dirSR.push_back(argv[4]);
   dirSR.push_back(argv[5]);
  
   const std::string fileSatG(argv[6]);
   unsigned int nbBand = atoi(argv[7]);
   float percentage=atof(argv[8]);
  
   SatRSRPointerType  satRSR=SatRSRType::New();
   /** Set the satelite number of bands */
   satRSR->SetNbBands(nbBand);
   /** Load the satelite response file*/
   satRSR->Load(fileSatG);


   //divide into training and testing files
   //90% of files are used for training and 10% for testing
   std::vector<ossimFilename> trainingFiles; //contains training files for all classes
   std::vector<unsigned int> trainingClasses; //contains training classes for each files
   std::vector<ossimFilename> testingFiles; //contains testing files for this classes
   std::vector<unsigned int> testingGTClasses; //contains testing ground truth classes for each files
   unsigned int ind;
   const ossimString regularExpressionPattern=".*\\.txt$";
   int flags = ossimDirectory::OSSIM_DIR_DEFAULT;
  
   for(unsigned int i=0;i<dirSR.size();i++) //for each class (directory)
   {
      std::vector<ossimFilename> result;
      ossimDirectory * directory = new ossimDirectory();
      std::cout<<"dirSR["<<i<<"] : "<<dirSR[i]<<std::endl;
      directory->open(dirSR[i]);
      directory->findAllFilesThatMatch(result, regularExpressionPattern, flags);
            
      delete(directory);
      directory=NULL;
      
      std::vector<ossimFilename> training; //contains training files for this class (directory)
      std::vector<ossimFilename> testing; //contains testing files for this class (directory)
      training=result;
      srand((unsigned int)getpid()); //init random
      for(unsigned int j=0;j<int(percentage*result.size());j++)
      {
         ind = rand()%(result.size());
         testing.push_back(result[ind]);
         training.erase(training.begin()+(ind-j));
      }
      
      //add to global training files and testing files
      for(unsigned int k=0;k<testing.size();k++)
      {
         std::cout<<"testing["<<k<<"] : "<< testing[k] <<std::endl;
         testingFiles.push_back(testing[k]);
         testingGTClasses.push_back(i);
      }
      
      for(unsigned int l=0;l<training.size();l++)
      {
         std::cout<<"training["<<l<<"] : "<< training[l] <<std::endl;
         trainingFiles.push_back(training[l]);
         trainingClasses.push_back(i); //class is the directory number
      }
    }

   //compute spectral response for all training files
   SampleListType::Pointer sampleList = SampleListType::New();
   TrainingSampleListType::Pointer trainingList = TrainingSampleListType::New();
   for(unsigned int i=0;i<trainingFiles.size();i++)
   {
      ResponsePointerType  spectralResponse=ResponseType::New();
      std::cout<<"training file : "<<trainingFiles[i]<<std::endl;
      spectralResponse->Load(trainingFiles[i], 100.0);
         
      //Compute Reduce Spectral Response
      ReduceResponseTypePointerType  reduceResponse=ReduceResponseType::New();
      /** Load the satelite response in the simulator */
      reduceResponse->SetInputSatRSR(satRSR);
      /** Load the spectral response of the object in the simulator*/
      reduceResponse->SetInputSpectralResponse(spectralResponse);
      reduceResponse->CalculateResponse();
      
      //Get the response in an itk::VariableLengthVector and add it to the sample list for SVMModelEstimator
      SampleType sample;
      TrainingSampleType trainingSample;
      sample.SetSize(reduceResponse->GetReduceResponse().size());
      std::cout<<"reduce response : [";
      for(unsigned int j=0;j<reduceResponse->GetReduceResponse().size();j++)
      {
         sample[j]=reduceResponse->GetReduceResponse()[j];
         std::cout<<reduceResponse->GetReduceResponse()[j]<<" ";
      }
      std::cout<<"]"<<std::endl;
      sampleList->PushBack(sample);
      trainingSample=trainingClasses[i];
      std::cout<<"training class : "<<trainingSample<<std::endl;
      trainingList->PushBack(trainingSample);
   }

   //SVM model estimator
   SVMModelEstimatorType::Pointer estimator = SVMModelEstimatorType::New();
   estimator->SetInputSampleList(sampleList);
   estimator->SetTrainingSampleList(trainingList);
   estimator->SetNu(0.5);
   estimator->SetKernelGamma(1);
   estimator->SetKernelCoef0(1);
   estimator->SetC(1);
   estimator->SetEpsilon(0.001);
   estimator->SetP(0.1);
   estimator->DoProbabilityEstimates(true);
   
   estimator->Update();
   estimator->GetModel()->SaveModel("model.txt");
  
  
   //compute spectral response for testing files
   sampleList->Clear();//clear the sample list to re use it for testing samples
   TrainingSampleListType::Pointer groundTruthClassList = TrainingSampleListType::New();
   for(unsigned int i=0;i<testingFiles.size();i++)
   {
      ResponsePointerType  spectralResponse=ResponseType::New();
      std::cout<<"testing file : "<<testingFiles[i]<<std::endl;
      spectralResponse->Load(testingFiles[i], 100.0);
         
      //Compute Reduce Spectral Response
      ReduceResponseTypePointerType  reduceResponse=ReduceResponseType::New();
      /** Load the satelite response in the simulator */
      reduceResponse->SetInputSatRSR(satRSR);
      /** Load the spectral response of the object in the simulator*/
      reduceResponse->SetInputSpectralResponse(spectralResponse);
      reduceResponse->CalculateResponse();
      
      //Get the response in an itk::VariableLengthVector and add it to the sample list for SVMClassifier
      SampleType sample;
      TrainingSampleType gtClass;
      sample.SetSize(reduceResponse->GetReduceResponse().size());
      for(unsigned int j=0;j<reduceResponse->GetReduceResponse().size();j++)
      {
         sample[j]=reduceResponse->GetReduceResponse()[j];
      }
      sampleList->PushBack(sample);
      gtClass=testingGTClasses[i];
      groundTruthClassList->PushBack(gtClass);
   }
   
   //SVM Classifier
   SVMClassifierType::Pointer classifier = SVMClassifierType::New();
   classifier->SetModel(estimator->GetModel());
   classifier->SetSample(sampleList);
   classifier->SetNumberOfClasses(dirSR.size());
   classifier->Update();
   
   ClassifierOutputType::ConstIterator it = classifier->GetOutput()->Begin();
   
   TrainingSampleListType::Pointer classifierListLabel = TrainingSampleListType::New();
   while(it != classifier->GetOutput()->End())
   {
         std::cout<<"class : "<<it.GetClassLabel()<<std::endl;
         classifierListLabel->PushBack(it.GetClassLabel());
         ++it;
   }
   for(unsigned int i=0;i<testingFiles.size();i++)
   {
      std::cout<<"ground truth class : "<<testingGTClasses[i]<<std::endl;
   }

   //Compute confusion matrix
   ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();
   confMatCalc->SetReferenceLabels(groundTruthClassList);
   confMatCalc->SetProducedLabels(classifierListLabel);
   confMatCalc->Update();
   
   std::cout<<std::endl;
   std::cout<<"Confusion matrix: "<<std::endl<< confMatCalc->GetConfusionMatrix()<<std::endl<<std::endl;
   std::cout<<"Kappa Index: "<<std::endl<< confMatCalc->GetKappaIndex()<<std::endl<<std::endl;
   
   return EXIT_SUCCESS;
}
