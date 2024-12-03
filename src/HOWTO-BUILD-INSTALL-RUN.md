# WebSocket SCADA / Server (logger)

## REQUIREMENTS

libraries

- Qt5Core
  - libqt5core5 libqt5core5-dev
- Qt5WebSockets
  - libqt5websockets5 libqt5websockets5-dev
- Qt5SerialPort
  - libqt5serialport5 libqt5serialport5-dev
- QtSolutions_Service-headd
- ModBus client
  - llibmodbus-5 lmodbus
- MySQL client
  - llibmysql lmysqlclient


## BUILD

### on Windows

over console
```
qmake
make
```
or over Qt Creator

### on Linux

over console
```
mkdir /opt/builds
cp libmodbus-x.x.x /opt/builds
cp mysql-connector-c-x.x.x-src /opt/builds
cp qtservice /opt/builds
```

make libmodbus
```
cd libmodbus-x.x.x
./configure --prefix=/usr
make
make install
  or
cp src/.libs/libmodbus.* /lib
cp src/.libs/libmodbus.* /lib64
cp src/.libs/libmodbus.* /opt/lib
```

make qtservice (only for OS Windows)
> if you have systemd, then it not needed
> disable support service in global.h of the project
```
cd qtservice
qmake
make
cp src/.libs/QtSolutions_Service-head.* /lib
cp src/.libs/QtSolutions_Service-head.* /lib64
cp src/.libs/QtSolutions_Service-head.* /opt/lib
```

make mysql-connector-c-x.x.x-src
```
cd mysql-connector-c-x.x.x-src
cmake -G "Unix Makefiles"
make
cp libmysql/libmysqlclient.* /lib
cp libmysql/libmysqlclient.* /lib64
cp libmysql/libmysqlclient.* /opt/lib
vi /etc/ld.so.conf.d/opt.conf
add /opt/lib
sudo ldconfig
```

make project
```
qmake
make
```


## RUN

```
server --config PathToConfigFile [ --sname ServiceName --sdesc ServiceDescription ]

where, PathToConfigFile - path to main config
            ServiceName - service name (optional)
     ServiceDescription - service description (optional)
```

### on Windows

```
modify script for-win/sc-install.bat
execute for-win/sc-install.bat
service Start | Stop (see Windows services)
```

### on Linux

```
modify file for-unix/wsloggerd.service
sudo cp for-unix/wsloggerd.service /etc/systemd/system
systemctl enable wsloggerd
systemctl start wsloggerd
systemctl start | status | stop
```
