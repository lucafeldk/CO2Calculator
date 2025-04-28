
# CO2Calculator 
CO2-Emissions are the main cause of man-made climate change. The biggest amount of  CO2 emissions in the world are emitted through the production of electricity. Per produced kilowatt-hour a certain amount of gram CO2 is emitted. This tool aims to visualize these emissions based on ENTSO-E data.

## CO2 Emissionsfactor per Production Type
Every energy prodcution type emmits a different amount of CO2 per kwh. Obviously fossil Energy sources like brown coal, hard coal or oil emitts more CO2 than greener alternatives like Wind or Solar Energy. The [ENTSO-E Transperency Platform](https://transparency.entsoe.eu/) provides for each energy production type the amount of power that is generated in an 15-minute Intervall. If you assign each of this production types an emmisionfactor you are able to calculate the emission per production type:

| Code | Production Type                            | emssionfactor in gCO2eq/kwH |
|------|----------------------------------------|-------------|
| B01  | Biomass                                | 266,8       |
| B02  | Lignite Coal                           | 1209,9      |
| B03  | Coal-derived gas                       | 952         |
| B04  | Fossil Gas                             | 467,4       |
| B05  | Hard coal                              | 1051,5      |
| B06  | Oil                                    | 890         |
| B07  | Oil shale                              | N/A        |
| B08  | Fossil Peat                            | N/A         |
| B09  | Geothermal                             | 182,6       |
| B10  | Hydro Pumped Storage                   | 25,1        |
| B11  | Hydro Run-of-river and poundage       | 2,7         |
| B12  | Hydro Water Reservoir                  | 25,1        |
| B13  | Marine                                 | N/A        |
| B14  | Nuclear                                | 32          |
| B15  | Other renewable                        | 93          |
| B16  | Solar                                  | 66,7        |
| B17  | Waste                                  | 266,8       |
| B18  | Wind Offshore                          | 10,5        |
| B19  | Wind Onshore                           | 6           |
| B20  | Other                                  | 683,8       |


## About the App
The desktop App is coded in C++ and at the moment uses a simple gui created with the qt framework. The app request the data via an http request to the ENTSO-E Platform API based on the settings that are chosen in the gui. With the data of the generated Power of the chosen production types the corresponding CO2 Emission in tons are calculated based on the table above. After this the data will be stored in a sqllite database and can be fetched from the frontend. Here is the Screenshot thats shows the Visualization of the production types Fossil Brown Coal and Fossil Gas for a few days:
![Image](https://github.com/user-attachments/assets/8fbb1669-6435-43af-8779-b1967c5861c3)

---

## Getting Started - Release 1.0.0
You can try the release. The functionality is still very rudementary. If you want to try nonetheless follow the steps below.

1. Download and extract the ZIP archive.
2. Copy `config_example.json` and rename it to `config.json`.
3. Enter your **ENTSO-E API key** (see below).
4. Launch `CO2Calculator.exe`.

[🔽 Download ZIP](https://github.com/lucafeldk/CO2Calculator/releases/download/v1.0.0/CO2Calculator_v1.0.0_Windows.zip)

---

## How to Get an ENTSO-E API Key

1. Visit: https://www.entsoe.eu
2. Create a free user account.
3. Under your account, request an API key for the ENTSO-E Transparency Platform.
4. The key will be sent to you via email.

💡 Activation may take up 1–2 business days. But my key i got after in a few hours after the request.

---

## Sample `config.json`

```json
{
  "api_key": "YOUR_API_KEY_HERE",
  "base_url": "https://web-api.tp.entsoe.eu/api"
}
```

📁 This file must be placed in the **same directory** as the `.exe` file.
