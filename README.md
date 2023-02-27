# Sistema de Control de Asistencia de la Fototeca Nacional
Programa escrito en C++ con el framework de Qt y la librería QZXing para registrar a los empleados de la Fototeca Nacional en una base de datos SQLite, permitir sus checadas de entrada y salida mediante códgios QR y generar reportes de sus capturas.

## Características
- Registro de empleados en una tabla
- Registro de sus horarios mediante horas específicas y selección de días de descanso
- Registro de su código QR por cámara o por texto
- Cualquier cambio se puede descartar
- Al reconocer un código QR el horario de captura se guarda en una base de datos
- Al reconocer el código se muestra información de los horarios del empleado de ese día
- Historial de capturas que se puede ordenar mediante año, mes, quincena, semana, día, o cualquier rango entre dos días
- Exportación del historial a .csv
- Modificación libre de los tipos de contratación

## Instalación
Este programa usa la fuente Montserrat, por lo que es recomendable instalarla previamente.
### Windows
1. Descargar el archivo comprimido AsistenciaFototeca.zip de los lanzamientos.
2. Ejecutar el programa AsistenciaFototeca.exe.
3. Se puede hacer un acceso directo del .exe y colocarlo donde se desee (el programa no ejecutará si no está dentro de la carpeta original).
