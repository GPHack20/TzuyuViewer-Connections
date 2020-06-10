# TzuyuViewer Connections Linux

Visor gráfico de redes para Linux usando Qt framework

[IMPORTAR IMAGEN]

## Descripción
TzuyuViewer Connections Linux: Es un visor gráfico para las conexiones de red para entornos GNU Linux, programado en Qt C++
```
Names – Mostrar línea de comando, nombre del proceso que posee el punto final.
Pause - Deja de cargar nuevos registros.
Record - No elimine registros antiguos.
Copy - Copiar los datos de una fila en el portapapeles.
Save – Exportar datos a un archivo CSV, HTML o XML.
Whois - El servicio le brinda la capacidad de averiguar el titular del dominio registrado.
Zoom slider - Le ayudará a seleccionar el tamaño de fuente apropiado.
```
## ¿Como funciona?
### "Base module"
Lea y analice la información disponible en:
```
    /proc/net/tcp
    /proc/net/udp
    /proc/net/tcp6
    /proc/net/udp6
    /proc/net/raw
    /proc/net/raw6
```
Los datos utilizados de la salida son:
```
sl: el número de la línea en la lista de salida.
local_address - Dirección local.
rem_address - Direccion remota
st: el estado del socket.
Uid: el ID del usuario propietario del socket.
Inode: un número de aspecto críptico que identifica el socket del sistema de archivos virtual de Linux.
```

### "Modulo raíz"
Recorrer  /proc/pid/fd/ para recopilar "inode list" y datos asociados de "command line".

# ¿Como contruirlo?

## Instalar las dependencias de compilacion.

```bash
$ sudo apt-get install build-essential
$ sudo apt-get install qtcreator
$ sudo apt-get install qt5-default
$ sudo apt-get install uuid-dev
```

## Clonar

```bash
$ git clone https://github.com/GPHack20/TzuyuViewer-Connections.git
```

## Contruir

Abrir el archivo `tcpview.pro` con Qt Creator para editar y/o contruirlo.

## Correr

```bash
$ ./tcpview
```
