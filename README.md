# Wireless temperature and humidity sensor

Based on Wemos shields: [D1 mini pro](https://www.wemos.cc/product/d1-mini-pro.html) - [SHT30 shield](https://www.wemos.cc/product/sht30-shield.html) - [Battery shield](https://www.wemos.cc/product/battery-shield.html).

The temperature, humidity and voltage data are sent on MQTT broker (like [mosquitto](https://mosquitto.org/)) with WiFi, so you can grab the data to push it where you want. 

I personally use [Telegraf](https://docs.influxdata.com/telegraf/v1.2/introduction/getting_started/), [Grafana](http://grafana.org/) and [home-assistant](https://home-assistant.io/) with this sensor.

Support of deepsleep.

## Built With

* [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi) - Enables network connection (local and Internet) using the ESP8266 built-in WiFi.
* [PubSubClient](http://pubsubclient.knolleary.net/) - Arduino Client for MQTT
* [WEMOS_SHT3X](https://github.com/wemos/WEMOS_SHT3x_Arduino_Library) - Arduino library for the WEMOS SHT30 Shiled

## Authors

* **Romain Boulanger** - [*www.romainboulanger.fr*](http://www.romainboulanger.fr) - [*www.twitter.com/dotromain*](https://twitter.com/dotromain)

## License

This project is licensed under GPLv3 License - see the [LICENSE.md](LICENSE.md) file for details
