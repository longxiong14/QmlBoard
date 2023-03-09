#include "henummodel.h"

HEnumModel::HEnumModel(QObject *parent) : QObject(parent) {
  setBorderTypesList({"BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT",
                      "BORDER_WRAP", "BORDER_REFLECT_101", "BORDER_TRANSPARENT",
                      "BORDER_REFLECT101", "BORDER_DEFAULT",
                      "BORDER_ISOLATED"});
  setBorderTypes({
      {"BORDER_CONSTANT", cv::BorderTypes::BORDER_CONSTANT},
      {"BORDER_REPLICATE", cv::BorderTypes::BORDER_REPLICATE},
      {"BORDER_REFLECT", cv::BorderTypes::BORDER_REFLECT},
      {"BORDER_WRAP", cv::BorderTypes::BORDER_WRAP},
      {"BORDER_REFLECT_101", cv::BorderTypes::BORDER_REFLECT_101},
      {"BORDER_TRANSPARENT", cv::BorderTypes::BORDER_TRANSPARENT},
      {"BORDER_REFLECT101", cv::BorderTypes::BORDER_REFLECT101},
      {"BORDER_DEFAULT", cv::BorderTypes::BORDER_DEFAULT},
      {"BORDER_ISOLATED", cv::BorderTypes::BORDER_ISOLATED},
  });
}
