#include <iostream>
#include "MyersDiffMatch.hpp"

using namespace differ;

int main(int argc, char** argv)
{
    String text1 = "My name is Eugene";
    String text2 = "My Name is Eugene";

    MyersDiffMatch diff;
    diff.ComputeDiff(text1, text2);

    return 0;
}