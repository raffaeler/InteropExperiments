use std::{env, fs, thread, time, time::Duration};
// use std::fs;
// use std::time::Duration;
use windows::{Interface};   // , Result

use bindings::{
    ManagedComponent::*,
    Windows::Foundation::*,
};


fn main() -> windows::Result<()> {
    let current_dir = env::current_dir().unwrap();
    println!("current: {:?}", current_dir);

    println!("\r\n\r\nTesting Calculator component");
    let calculator = Calculator::new()?;
    let res = calculator.Add(1.2, 3.3)?;
    println!("Calculator result is {}", res);


    println!("\r\n\r\nTesting Echo component");
    let echo = Echo::new()?;
    let echo_response1 = echo.SendReceive("Hi there!")?;
    println!("{}", echo_response1);


    println!("\r\n\r\nTesting Alarm component");
    let alarm = Alarm::new()?;
    let echo_response2 = alarm.SendReceive("Hi from Alarm!")?;
    println!("{}", echo_response2);


    let eh = EventHandler::<AlarmEventArgs>::new(
                    move |s, a| {
        let u = s.as_ref().unwrap();
        let sender : Alarm = u.cast()?;

        let  response3 = sender.SendReceive("Hi from inside an event").unwrap();
        println!("{:?}", response3);

        let arg = a.as_ref().unwrap();
        println!("Event!: {:?} {:?}", sender, arg.Message());
        Ok(())
    });


    let token = alarm.Elapsed(eh)?;
    let tspan : TimeSpan = Duration::from_millis(200).into();
    alarm.Start(tspan, "Hello, Rust")?;
    thread::sleep(time::Duration::from_millis(500));

    alarm.RemoveElapsed(token)?;

    println!("\r\n\r\nTesting QueryCatalog component");

    let xml = fs::read_to_string("cd.xml")
        .expect("Something went wrong reading the file");
    let catalog = QueryCatalog::new()?;
    let title = catalog.MakeQuerySingle(xml.clone(), "year", "1987", "title")?;
    println!("Single Query result: {}\r\n", title);

    println!("Multiple Query result:");
    let list = catalog.MakeQueryMultiple(xml.clone(), "year", "1987", "title")?;
    for item in list {
        println!("   {}", item);
    }


    Ok(())
}
