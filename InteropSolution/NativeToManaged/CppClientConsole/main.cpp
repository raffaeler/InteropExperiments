#include "pch.h"
#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;

int main()
{
    init_apartment();

    CalcComponent::Calculator calc;
    std::cout << "calc " << calc.Add(2.0, 3.5) << std::endl;

    //ChronoComponent::Alarm alarm;


    Uri uri(L"http://aka.ms/cppwinrt");
    printf("Hello, %ls!\n", uri.AbsoluteUri().c_str());
}
