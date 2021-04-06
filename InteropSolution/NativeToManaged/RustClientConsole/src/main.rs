use bindings::{
    calc_component::Calculator,
    chrono_component::Echo,

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

    let calculator = Calculator::new()?;
    let res = calculator.add(1.2, 3.3)?;

    println!("Calculator result is {}", res);

    let echo = Echo::new()?;
    let echoed = echo.send_receive("Ping")?;
    println!("{}", echoed);
/*
    let red = Colors::red()?;
    println!("Red: {:?}", red);
*/
    Ok(())
}
