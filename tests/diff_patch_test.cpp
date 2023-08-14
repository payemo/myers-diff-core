#include <iostream>
#include "Patch.hpp"

using namespace differ;

int main(int argc, char** argv)
{
    String text1 = "My name is Eugene";
    String text2 = "my name is Eugene";
    
    Sequence s1(text1), s2(text2);
    Diff d1(Operation::INSERT, s1);
    Diff d2(Operation::DELETE, s2);
    Patch p;
    p.Append(d1);
    p.Append(d2);

    String res = p.ToString();

    return 0;
}