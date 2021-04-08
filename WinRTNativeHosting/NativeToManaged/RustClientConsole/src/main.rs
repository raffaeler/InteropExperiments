use std::env;
use std::fs;

use bindings::{
    ManagedComponent::*,

    //windows::Result,
    //windows::ui::Colors,
    //windows::ui::Color

    // windows::foundation::Uri,
    // windows::web::syndication::SyndicationClient,
};

// use bindings::*;
// use winrt::Result;
// use windows::application_model::activation;


fn main() -> windows::Result<()> {
    // let uri = Uri::create_uri("https://kennykerr.ca/feed")?;
    // let client = SyndicationClient::new()?;
    // let feed = client.retrieve_feed_async(uri)?.get()?;

    // for item in feed.items()? {
    //     println!("{}", item.title()?.text()?);
    // }

    let current_dir = env::current_dir().unwrap();
    //let joined = Path::new(&current_dir).join("cd.xml").canonicalize().unwrap();
    //let itself = Path::new("cd.xml").canonicalize().unwrap();
    println!("current: {:?}", current_dir);
    //println!("joined: {:?}", joined);
    //println!("joined: {:?}", itself);



    let calculator = Calculator::new()?;
    let res = calculator.Add(1.2, 3.3)?;

    println!("Calculator result is {}", res);

    let echo = Echo::new()?;
    let echo_response1 = echo.SendReceive("Echo component")?;
    println!("{}", echo_response1);

    let alarm = Alarm::new()?;
    let echo_response2 = alarm.SendReceive("Alarm component")?;
    println!("{}", echo_response2);

    //alarm.Elapsed(handler)
    //let arg = AlarmEventArgs::new()?;
    //arg.


    let xml = fs::read_to_string("cd.xml")
        .expect("Something went wrong reading the file");
    let catalog = QueryCatalog::new()?;
    let title = catalog.MakeQuerySingle(xml.clone(), "year", "1987", "title")?;
    println!("single item: {}", title);

    let list = catalog.MakeQueryMultiple(xml.clone(), "year", "1987", "title")?;
    for item in list {
        println!("item: {}", item);
    }

/*
    let red = Colors::red()?;
    println!("Red: {:?}", red);
*/
    Ok(())
}
