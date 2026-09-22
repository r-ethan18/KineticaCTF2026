#include <string>
#include <iostream>

#include "picosha2.h"

std::string chunk0 = "53616c746172";
std::string chunk1 = "655f655f6469";
std::string chunk2 = "66666963696c";
std::string chunk3 = "655f6d615f69";
std::string chunk4 = "6f5f736f6e6f";
std::string chunk5 = "5f4d6172696f";

bool first_correct = false;
bool second_correct = false;
bool third_correct = false;
bool fourth_correct = false;
bool fifth_correct = false;
bool sixth_correct = false;

void check_first(std::string value, bool *valid) {
  std::string hash_here =
}