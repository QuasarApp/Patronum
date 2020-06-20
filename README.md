# Patronum
This is extension libraries for control your daemons.

### Why is Patronum?
Becouse This library offers easy interface to control your demons likewise the magic of Harry Potter controls dementors

## Main features

* Support linux systemd daemons.
* Auto create a Service from your server or yor daemon utility.
* Auto create a Controller of your Service. 

## Include


### For cmake projects
 
 * cd yourRepo
 * git submodule add https://github.com/QuasarApp/Patronum.git # add the repository of Patronum into your repo like submodule
 * git submodule update --init --recursive
 * Include in your CMakeLists.txt file the main CMakeLists.txt file of Patronum library
  ``` cmake
    add_subdirectory(Patronum)
  ```
 * Rebuild yuor project



## Use

### Service 
``` cpp
#include <patronum.h>

class MyserviceApp : public Patronum::Service<QCoreApplication>
{

public:
    MyserviceApp(int argc, char **argv):
        Patronum::Service<QCoreApplication>(argc, argv, "MyService") {

    }

    void start() {
        // call on server started 
    }

    void stop() {
        // call on server stoped 
    }

    void handleReceive(const QList<Feature> &data) {
        for (auto i : data) {
            if (i.cmd == "Ping") {
                sendResuylt(QVariantMap{{"Pong", "From server"}})
            }
        }
    };


    QList<Feature> supportedFeatures() {

        QList<Feature> res;
        Feature Ping = {"Ping", ""}
        return res << Ping;
    }
};


int main(int argc, char **argv) {
    MyserviceApp app;
    return app.exec();
}
```
### Controller
``` cpp
#include <patronum.h>

class MyControllerApp : public Patronum::Controller
{
public:

    MyControllerApp():
        Patronum::Controller("MyService") {

    }
};

int main(int argc, char **argv) {
    QCoreApplcication app
    MyControllerApp controller;
    controller.send(argc, argv);
    return app.exec();
}
```
