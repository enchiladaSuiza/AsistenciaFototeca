# Sistema de Control de Asistencia de la Fototeca Nacional
Programa escrito en C++ con el framework de Qt y la librería QZXing para registrar a los empleados de la Fototeca Nacional en una base de datos SQLite, permitir sus checadas de entrada y salida mediante códgios QR y generar reportes de sus capturas.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Características
- Registro de empleados en una tabla.
- Registro de sus horarios mediante horas específicas y selección de días de descanso.
- Registro de su código QR por cámara o por texto.
- Cualquier cambio se puede descartar.
- Al reconocer un código QR el horario de captura se guarda en una base de datos.
- Al reconocer el código se muestra información de los horarios del empleado de ese día.
- Historial de capturas que se puede ordenar mediante año, mes, quincena, semana, día, o cualquier rango entre dos días.
- El historial calcula y muestra el total de minutos de demora de entrada, salida y faltas.
- Exportación del historial a .csv.
- Sistema de usuarios a los que se pueden asignar permisos de acceso a las pantallas del sistema.
- Opciones para definir los tipos de contratación y minutos de tolerancia.

## Instalación
Este programa usa la fuente Montserrat, por lo que es recomendable instalarla previamente.
### Windows
1. Descargar el archivo comprimido AsistenciaFototeca.zip de los lanzamientos y extraer los archivos en una carpeta.
2. Ejecutar el programa AsistenciaFototeca.exe.
3. Se puede hacer un acceso directo del .exe y colocarlo donde se desee (el programa no se puede mover de su carpeta original, de lo contrario no ejecutará).

## Compilación
### Windows
1. Este programa es compilado con la versión 6.2.4 de Qt, por lo que primero se necesita instalar ese kit a través de la Qt Maintenance Tool.
2. Clonar el repositorio.
3. Abrir el archivo AsistenciaFototeca.pro dentro de Qt Creator y asignarle el kit de Qt 6.2.4.
4. Construir y ejecutar.
