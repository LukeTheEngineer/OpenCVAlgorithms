#pragma once
//System includes
#include<iostream>
#include<cstring>
#include<string>
#include<stdlib.h>

//Opencv includes
#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>

class ImageProcessor{
public:
    ImageProcessor(const std::string& inputFile, const std::string& outputFile)
        : inputFile(inputFile), outputFile(outputFile) {}

    ImageProcessor(const std::string& inputFile)
        : inputFile(inputFile) {}

    ImageProcessor() {}

    /// <summary>
    /// Algorithm that alters an image by using brightness and contrast params
    /// </summary>
    /// <param name="inputFile ">Input a directory including the file</param>
    /// <param name="brightness ">Used to indicate brightness level</param>
    /// <param name="contrast ">Used to indicate contrast level</param>
    /// <returns>**img** cv::Mat</returns>
    cv::Mat EditImage(const cv::String& inputFile, int brightness = 0, int contrast = 0) {
        cv::Mat img = cv::imread(inputFile, cv::IMREAD_GRAYSCALE);
        try {
            if (img.empty()) {
                throw std::invalid_argument("Input image cannot be loaded. ");
            }
            if (contrast != 0) {
                double f = 131 * (contrast + 127) / (127 * (131 - contrast));
                double alpha_c = f;
                double gamma_c = 127 * (1 - f);
                cv::Mat lookUpTable(1, 256, CV_8U);
                uchar* lut = lookUpTable.ptr();
                for (int i = 0; i < 256; i++) {
                    lut[i] = cv::saturate_cast<uchar>(alpha_c * i + gamma_c);
                }
                cv::LUT(img, lookUpTable, img);
            }
            if (brightness != 0) {
                img += brightness;
            }
        }
        catch (const cv::Exception& Ex)
        {
            const char* _Enhancing_Exception_Handle = Ex.what();
            std::cerr << "Exception caught in '_EnhanceImage' function at line " << __LINE__ << ": " << _Enhancing_Exception_Handle << std::endl;
            return cv::Mat(); // return empty cv::Mat object
        }
        catch (const std::runtime_error& Ex)
        {
            const char* _Enhancing_Runtime_Handle = Ex.what();
            std::cerr << "Runtime error detected in '_EnhanceImage' function at line " << __LINE__ << "Runtime Error message: " << _Enhancing_Runtime_Handle;
            return cv::Mat(); // return empty cv::Mat object
        }
        catch (const std::invalid_argument& IArg) {
            const char* _Invalid_Argument_Handle = IArg.what();
            std::cerr << "Invalid argument: " << _Invalid_Argument_Handle << std::endl;
            return cv::Mat(); // return empty cv::Mat object
        }

        return img; // return processed cv::Mat object
    }

    /// <summary>
    /// Enhance the image by using sigma_s and sigma_r params
    /// </summary>
    /// <param name="inputFile">Input a directory including the file</param>
    /// <param name="sigma_s">Smooths the image using user input</param>
    /// <param name="sigma_r">Preserves image while smoothing</param>
    /// <returns>**result** cv::Mat</returns>
    cv::Mat _EnhanceImage(const cv::String& inputFile, float sigma_s, float sigma_r) {
        cv::Mat img = cv::imread(inputFile, cv::IMREAD_COLOR);
        cv::Mat result;
        cv::Mat outputImage;
        try {
            if (img.empty()) {
                throw std::invalid_argument("Input image can't be loaded. ");
            }
            cv::detailEnhance(img, result, sigma_s, sigma_r);
            result.convertTo(outputImage, CV_8UC3);
        }
        catch (const cv::Exception& Ex) {
            const char* Exception_Handle = Ex.what();
            std::cerr << "Exception caught in 'detailEnhance' function at line: " << __LINE__ << Exception_Handle << std::endl;
            return cv::Mat();
        }
        catch (const std::invalid_argument& IEx) {
            const char* Invalid_Arg_Handle = IEx.what();
            std::cerr << "Invalid Argument detected at line: " << __LINE__ << " Message: " << Invalid_Arg_Handle << std::endl;
            return cv::Mat();
        }
        return result;
    }

    /// <summary>
    /// An algorithm that detects dark spots in an image and brightens them
    /// </summary>
    /// <param name="inputFile">directory and file to apply this to</param>
    /// <returns>**output** cv::Mat</returns>
    cv::Mat BrightnessAlgorithm(const cv::String& inputFile) {
        std::cout << "Brightness Algorithm invoked\n\n";
        cv::Mat image = cv::imread(inputFile);
        cv::Mat gray, binary, mask, output;
        try {
            if (image.empty()) {
                std::cout << "Could not load image. \n"<<std::endl;
            }
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
            cv::threshold(gray, binary, 50, 255, cv::THRESH_BINARY);
            cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
            cv::erode(binary, binary, kernel);
            cv::dilate(binary, binary, kernel);
            cv::GaussianBlur(binary, binary, cv::Size(9, 9), 0);
            cv::cvtColor(binary, mask, cv::COLOR_GRAY2BGR);
            cv::bitwise_and(image, mask, output);
            cv::multiply(output, cv::Scalar(1.2, 1.2, 1.2), output);

            cv::waitKey(0);
        }
        catch (const cv::Exception& Ex)
        {
            const char* Exception_Handler = Ex.what();
            std::cerr << "Exception caught on line " << __LINE__ << "\n Message: " << Exception_Handler << std::endl;
        }
        catch (const std::runtime_error& RErr)
        {
            const char* Runtime_Error_Handler = RErr.what();
            std::cerr << "Runtime error raised during compile..." << std::endl;
            std::cout << "Message: " << Runtime_Error_Handler << std::endl;
        }
        return output;
    }
private:
    std::string inputFile;
    std::string outputFile;
};

namespace Proccess {

    std::vector<cv::Mat> ImageProcess(cv::Mat(ImageProcessor::* funcPtr)(const cv::String&),
        ImageProcessor& obj, std::vector<cv::String>& imagePaths,
        cv::String fileName, cv::String outputDir)
    {
        std::vector<cv::Mat> resultImages;
        try {
            for (const auto& imagePath : imagePaths) {
                cv::Mat resultImage = (obj.*funcPtr)(imagePath);
                cv::String outputFilePath = outputDir + "/" + fileName;
                bool success = cv::imwrite(outputFilePath, resultImage, { cv::IMWRITE_JPEG_QUALITY, 90 });
                if (!success) {
                    std::cout << "Failed to write " << imagePath << '\n';
                }
                else {
                    std::cout << imagePath << " written successfully. \n";
                }
                resultImages.push_back(resultImage);
            }
        }
        catch (const cv::Exception& cvException) {
            std::cerr << "[Error 1] An error occurred: " << cvException.what() << '\n';
        }
        catch (const std::exception& stdException) {
            std::cerr << "[Error 1.0] An error occurred: " << stdException.what() << '\n';
        }
        return resultImages;
    }

    std::vector<cv::Mat> ImageProcess(cv::Mat(ImageProcessor::* funcPtr)(const cv::String&, float, float),
        ImageProcessor& obj, float sigmaS, float sigmaR, const std::vector<cv::String>& imagePaths,
        const cv::String& fileName, const cv::String outputDir) {
        std::vector<cv::Mat> resultImages;
        try {
            for (const auto& imagePath : imagePaths) {
                cv::Mat resultImage = (obj.*funcPtr)(imagePath, sigmaS, sigmaR);
                resultImages.push_back(resultImage);
                cv::String outputDirectory = outputDir + fileName;
                bool success = cv::imwrite(outputDirectory, resultImage, { cv::IMWRITE_JPEG_QUALITY, 90 });
                if (!success) {
                    std::cout << "Failed to write " << fileName;
                }
                else if (success) {
                    std::cout << fileName << " successful. \n";
                }
            }
        }
        catch (const cv::Exception& cvException) {
            const char* CV_Exception = cvException.what();
            std::cerr << "[Error 2] An error occurred. ";
        }
        catch (const std::exception& stdException) {
            const char* STD_Exception = stdException.what();
            std::cerr << "[Error 2] An error occurred. ";
        }
        return resultImages;
    }

    std::vector<cv::Mat> ImageProcess(cv::Mat(ImageProcessor::* funcPtr)(const cv::String&, int, int),
        ImageProcessor& obj, int Brightness, int contrast, const std::vector<cv::String>& imagePaths,
        const cv::String& fileName, const cv::String outputDir) {
        std::vector<cv::Mat> resultImages;
        try {
            for (const auto& imagePath : imagePaths) {
                cv::Mat resultImage = (obj.*funcPtr)(imagePath, Brightness, contrast);
                resultImages.push_back(resultImage);
                cv::String outputDirectory = outputDir + fileName;
                bool success = cv::imwrite(outputDirectory, resultImage, { cv::IMWRITE_JPEG_QUALITY, 90 });
                if (!success) {
                    std::cout << "Failed to write " << fileName;
                }
                else if (success) {
                    std::cout << fileName << " successful. \n";
                }
            }
        }
        catch (const cv::Exception& cvException) {
            const char* CV_Exception = cvException.what();
            std::cerr << "[Error] An error occurred. ";
        }
        catch (const std::exception& stdException) {
            const char* STD_Exception = stdException.what();
            std::cerr << "[Error] An error occurred. ";
        }
        return resultImages;
    }
}