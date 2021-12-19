# 📃 Sistema de reserva de máquinas
Grupo 202

Práctica de implementación en C++ para la asignatura de Ingeniería del Software. Grado en Ingeniería Informática (UCO).

## 👨🏻‍💻 Componentes del grupo:
- Rafael Carlos Díaz Mata
- Juan Antonio Gálvez Jiménez
- Mateo Fortea Dugo

## ⚙️ Recursos utilizados:
- IDE Eclipse
- Librería conector de BD para MySQL - [X DevAPI](https://dev.mysql.com/doc/x-devapi-userguide/en/)
- Servidor mySQL
- Plugin CUTE para testing

## 📝 Instrucciones de uso:
Para poder ejecutar este software, es necesario tener instalada la librería de cliente de mySQL (disponible en Windows, macOS y Linux). Para funcionar correctamente, debe ser ejecutado a través del terminal del sistema. 
````
-> Proyecto202: ./reservador
````
***Algunas de las funciones que hacen uso de comandos del sistema como "system(clear)" son orientadas para equipos que ejecutan Linux o macOS, por lo que pueden generar algún error en la plataforma Windows.***

### 🛠  Makefile
El Makefile encargado de compilar el proyecto, incluye un "path" llamado ```MYSQL_CONCPP_DIR```, que deberá contener la ruta de nuestro conector de mySQL para C++ instalado en nuestro sistema. En este caso, el utilizado en el proyecto es el siguiente:
```make
MYSQL_CONCPP_DIR = /usr/local/mysql-connector-c++-8.0.22
CPPFLAGS = -I $(MYSQL_CONCPP_DIR)/include -L $(MYSQL_CONCPP_DIR)/lib64
LDLIBS = -lmysqlcppconn8
CXXFLAGS = -std=c++11
app : app.cc
```
### ✅ Test Unitarios
Los tests unitarios han sido desarrollados a través de la librería CUTE instalada como plugin en Eclipse. Se encuentran como un proyecto aparte en la carpeta ```ReservadorTests```, donde se han testado tanto los constructores de las distintas clases, como distintos métodos de comprobaciones dentro del sistema.

### 🔌  Fichero de conexión
Para conectarse a la base de datos, se hace uso de un fichero como método de almacenamiento de las credenciales. Para que el sistema reconozca las credenciales, deberá crear un fichero de texto plano nombrado como "bd.txt" en la misma ruta que su ejecutable, dicho fichero contendrá las credenciales en este mismo orden:
```
HOST(dirección IP del servidor)
NOMBRE_DE_USUARIO
CONTRASEÑA
NOMBRE_DE_LA_BD
```
Por ejemplo:
```
localhost
usuario
1234
proyecto202
```

### 🗄 Script SQL
La base de datos utilizada puede obtenerse a partir del siguiente script SQL:
```sql

/* TABLA MÁQUINAS */
CREATE TABLE `maquinas` (
  `ID` int NOT NULL,
  `nombre_maquina` varchar(100) NOT NULL,
  `nucleos` int NOT NULL,
  `ram` int NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

/*/////////////////*/

/* TABLA RESERVAS */
CREATE TABLE `reservas` (
  `ID` int NOT NULL,
  `usuario` int NOT NULL,
  `maquina` int NOT NULL,
  `fecha_inicio` varchar(10) NOT NULL,
  `fecha_fin` varchar(10) NOT NULL,
  `num_nucleos` int NOT NULL,
  `motivo_reserva` varchar(500) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

/*/////////////////*/

/* TABLA USUARIOS */
CREATE TABLE `usuarios` (
  `ID` int NOT NULL,
  `nombre` varchar(100) NOT NULL,
  `apellidos` varchar(100) NOT NULL,
  `correo` varchar(100) NOT NULL,
  `pass` varchar(100) NOT NULL,
  `departamento` varchar(100) NOT NULL,
  `limite_maquinas` int NOT NULL,
  `limite_cpus` int NOT NULL,
  `limite_reservas` int NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

/*/////////////////*/

/* RESTRICCIONES DE CLAVES */
ALTER TABLE `maquinas`
  ADD PRIMARY KEY (`ID`);

ALTER TABLE `reservas`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `reservas_usuarios` (`usuario`),
  ADD KEY `reservas_maquinas` (`maquina`);
  
ALTER TABLE `usuarios`
  ADD PRIMARY KEY (`ID`);

/*/////////////////*/

/* CAMPOS CON AUTOINCREMENTO */
ALTER TABLE `maquinas`
  MODIFY `ID` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

ALTER TABLE `reservas`
  MODIFY `ID` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=18;

/*/////////////////*/

/* RELACIONES DE CLAVES */
ALTER TABLE `reservas`
  ADD CONSTRAINT `reservas_maquinas` FOREIGN KEY (`maquina`) REFERENCES `maquinas` (`ID`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  ADD CONSTRAINT `reservas_usuarios` FOREIGN KEY (`usuario`) REFERENCES `usuarios` (`ID`) ON DELETE RESTRICT ON UPDATE RESTRICT;
COMMIT;
```





