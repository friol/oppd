//
// oopd
// weather report
// friol 2o22
//

#include "include/oppd.h"

const std::string apiKey = "a3c3667c659c4d4e9826a8d981876a33";

bool getLocalIpAddress(std::string& ipaddr)
{
    //https://api.ipify.org?format=json

    httplib::Client cli("https://api.ipify.org");
    auto res = cli.Get("/");
    if (res->status != 200)
    {
        std::cout << "Error reading IP address from api.ipify.org." << std::endl;
        return false;
    }

    ipaddr = res->body;

    return true;
}

bool getGeolocation(std::string ipaddr, float& latitude, float& longitude)
{
    //https://api.ipgeolocation.io/ipgeo?apiKey=

    httplib::Client cli("https://api.ipgeolocation.io");
    auto res = cli.Get("/ipgeo?apiKey="+apiKey);
    if (res->status != 200)
    {
        std::cout << "Error geolocating IP address with api.ipgeolocation.io." << std::endl;
        return false;
    }

    std::string geoloc = res->body;
    json jgl= json::parse(geoloc);

    std::cout << "Location identified as " << jgl["city"] << "," << jgl["country_name"] << std::endl;

    std::string la = jgl["latitude"];
    std::string lo = jgl["longitude"];

    latitude = (float)atof(la.c_str());
    longitude= (float)atof(lo.c_str());

    return true;
}

bool getWeatherForecast(float lat, float longi, std::string& response,float& tempTomorrow,float& numOfRainHours,int& weathercode)
{
    //https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&daily=temperature_2m_max&timezone=GMT

    std::string slat = std::to_string(lat);
    std::string slong = std::to_string(longi);

    httplib::Client cli("https://api.open-meteo.com");
    auto res = cli.Get("/v1/forecast?latitude="+slat+"&longitude="+slong+"&daily=temperature_2m_max&timezone=GMT");
    if (res->status != 200)
    {
        std::cout << "Error getting weather forecast with api.open-meteo.com." << std::endl;
        return false;
    }

    response = res->body;
    json tempForecast = json::parse(response);
    tempTomorrow = tempForecast["daily"]["temperature_2m_max"][1];

    //

    res = cli.Get("/v1/forecast?latitude=" + slat + "&longitude=" + slong + "&daily=precipitation_hours&timezone=GMT");
    if (res->status != 200)
    {
        std::cout << "Error getting weather forecast with api.open-meteo.com." << std::endl;
        return false;
    }

    response = res->body;
    tempForecast = json::parse(response);
    numOfRainHours = tempForecast["daily"]["precipitation_hours"][1];

    //

    res = cli.Get("/v1/forecast?latitude=" + slat + "&longitude=" + slong + "&daily=weathercode&timezone=GMT");
    if (res->status != 200)
    {
        std::cout << "Error getting weather forecast with api.open-meteo.com." << std::endl;
        return false;
    }

    response = res->body;
    tempForecast = json::parse(response);
    weathercode = tempForecast["daily"]["weathercode"][1];

    return true;
}

std::string weathercodeToString(int code)
{
    std::string res = "";

    if (code == 0)
    {
        res = "Clear sky, sun will be shining";
    }
    else if ((code == 1) || (code == 2) || (code == 3))
    {
        res = "Mainly clear, partly cloudy, and overcast";
    }
    else if ((code == 45) || (code == 48))
    {
        res = "Fog and depositing rime fog";
    }
    else if ((code == 51) || (code == 53) || (code==55))
    {
        res = "Drizzle: Light, moderate, and dense intensity";
    }
    else if ((code == 56) || (code == 57))
    {
        res = "Freezing Drizzle: Light and dense intensity";
    }
    else if ((code == 61) || (code == 63)||(code==65))
    {
        res = "Rain: Slight, moderate and heavy intensity";
    }
    else if ((code == 66) || (code == 67))
    {
        res = "Freezing Rain: Light and heavy intensity";
    }
    else if ((code == 71) || (code == 73) || (code == 75))
    {
        res = "Snow fall: Slight, moderate, and heavy intensity";
    }
    else if (code == 77)
    {
        res = "Snow grains";
    }
    else if ((code == 80) || (code == 81) || (code == 82))
    {
        res = "Rain showers: Slight, moderate, and violent";
    }
    else if ((code == 85) || (code == 86))
    {
        res = "Snow showers slight and heavy";
    }
    else if (code == 95)
    {
        res = "Thunderstruck";
    }
    else if ((code == 96) || (code == 99))
    {
        res = "Thunderstorm with slight and heavy hail";
    }
    else
    {
        res = "Unknown weathercode, I'm sorry";
    }

    return res;
}

void day08usage(char** argv)
{
    std::cout << "Usage:" << std::endl << argv[0] << " [latitude] [longitude]" << std::endl;
}

void day08(int argc, char** argv)
{
    bool coordinatesInInput = false;
    float latitude, longitude;
    if (argc == 3)
    {
        latitude = (float)atof(argv[1]);
        longitude = (float)atof(argv[2]);
        //std::cout << "Entered coordinates: " << latitude << " " << longitude << std::endl;
        if ((latitude == 0) || (longitude == 0))
        {
            std::cout << "Error: please enter valid latitude and longitude coordinates" << std::endl;
            return;
        }
        coordinatesInInput = true;
    }
    else if ((argc > 3) || (argc != 1))
    {
        day08usage(argv);
        return;
    }

    if (coordinatesInInput == false)
    {
        // get external ip address
        std::string ipAddr;
        if (getLocalIpAddress(ipAddr) == false)
        {
            return;
        }

        std::cout << "Current external IP address: " << ipAddr << std::endl;

        // get geolocation from ip address

        if (getGeolocation(ipAddr, latitude, longitude) == false)
        {
            return;
        }

        std::cout << "Geolocated to lat: " << latitude << " long: " << longitude << std::endl;
    }

    // get weather forecast

    std::string resp;
    float tempTomorrow, numOfRainHours;
    int weathercode;

    if (getWeatherForecast(latitude, longitude,resp,tempTomorrow,numOfRainHours,weathercode) == false)
    {
        return;
    }

    std::string weatherString = weathercodeToString(weathercode);
    std::cout << "Weather forecast for tomorrow: average temperature " << tempTomorrow << " with " << numOfRainHours << " hours of rain." << std::endl;
    std::cout << "The elder says : " << weatherString << "." << std::endl;
}
