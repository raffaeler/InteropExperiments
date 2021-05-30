#include "pch.h"
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>

#include "Utilities.h"


using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
namespace fs = std::filesystem;


void test_alarm()
{
    ManagedComponent::Alarm alarm;
    auto response2 = to_string(alarm.SendReceive(L"Hello Echo"));
    cout << response2 << endl;

    //auto token = alarm.Elapsed([](IInspectable const& ii, const ManagedComponent::AlarmEventArgs& s2)
    //    {
    //        auto alarm = ii.as<ManagedComponent::Alarm>();

    //        auto response2 = alarm.SendReceive(L"message within the event");
    //        cout << to_string(s2.Message()) << endl;// << s2 << endl;
    //        alarm.Close();
    //    });

    auto handler1 = ([](IInspectable const& ii, const ManagedComponent::AlarmEventArgs& s2)
            {
                auto sender = ii.as<ManagedComponent::Alarm>();

                auto response2 = sender.SendReceive(L"message within the event");
                cout << to_string(s2.Message()) << endl;// << s2 << endl;
                sender.Close();
            });
    auto token1 = alarm.Elapsed(handler1);


    //Windows::Foundation::TypedEventHandler<ManagedComponent::Alarm, ManagedComponent::AlarmEventArgs> handler2(
    //    [](ManagedComponent::Alarm sender, ManagedComponent::AlarmEventArgs args)
    //    {
    //        auto response2 = sender.SendReceive(L"message within the event");
    //        cout << to_string(args.Message()) << endl;// << s2 << endl;
    //        sender.Close();
    //    });
    //auto token2 = alarm.Elapsed(handler2);

    TimeSpan ts = std::chrono::milliseconds{ 500 };
    alarm.Start(ts, L"Ping");
}

void test_query()
{
    auto xml = Utilities::load_text("cd.xml", fs::current_path());

    ManagedComponent::QueryCatalog cat;

    cout << "MakeQuerySingle" << endl;
    auto single = cat.MakeQuerySingle(to_hstring(xml), L"year", L"1987", L"title");
    cout << "  > " << to_string(single) << endl << endl;

    cout << "MakeQueryMultiple" << endl;
    auto list = cat.MakeQueryMultiple(to_hstring(xml), L"year", L"1987", L"title");

    for (const auto& item : list)
    {
        cout << "  > " << to_string(item) << endl;
    }
}

int main()
{
    //init_apartment(); // no more needed with the newest releases of cppwinrt

    ManagedComponent::Calculator calc;
    std::cout << "calc " << calc.Add(2.0, 3.5) << std::endl;

    ManagedComponent::Echo echo;
    auto response = to_string(echo.SendReceive(L"Hello Echo"));
    cout << response << endl;

    test_alarm();

    //cout << fs::current_path() << endl;
    test_query();

    std::cin.get();

    //Uri uri(L"http://aka.ms/cppwinrt");
    //printf("Hello, %ls!\n", uri.AbsoluteUri().c_str());
}
