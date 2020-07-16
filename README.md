# Introducción
Este proyecto corresponde con la asignatura de diseño de lenguajes de programación en la Universidad de La Coruña, en el
se ha desarrollado en el leguaje de programación C++ un interprete de lambda cálculo. 
Este intérprete soporta un subconjunto del su variante tipada, implementando tipado simple así como otras funcionalidades:

- __Naturales__
- __Strings__
- __Floats__
- __Bools__
- __Units__
- __Listas y concatenacion de estas__
- __Tuplas y acceso mediante indices__
- __Registros y acceso mediante etiqueta__
- __Condiciones if-then-else__
- __Simple pattern-matching__
- __Recursividad__
- __Alias__
- __Ascripciones__
 
Algunas limitaciones de esta práctica a tener en cuenta son:
 - Cada linea del terminal lee hasta que encuentra un salto de linea, esta limitacion viene dada por el interprete no por el parser ni el lexer. 
 Sin embargo se pueden leer ficheros externos con codigo mediante la operacion:  
 ```#open ruta_del_archivo;;```

- Las aplicaciones (por ejemplo: id 1) no soportan el estilo currificado,
      esto significa que solo podemos aplicar un termino a la vez, 
      para usar mas terminos necesitamos usar parentesis. Esta limitacion viene impuesta por el parser y se ha decidido 
      no soportar esta funcionalidad con el fin de evitar problemas de shift/reduce.
  - Ejemplo (mal): ```id2 1 2 3```  
  - Ejemplo (bien): ```(((id2 1) 2) 3)```  

# Estructura

La estructura del proyecto es la siguiente:
- **include:** Encabezados de c++

- **source:** Codigo fuente de c++ junto con el lexer y parser

- **CMakeLists.txt:** Archivo de cmake para compilar el proyecto

- **examples.lm:** Algunos ejemplos de las posibilidades que aporta este interprete

Algunos archivos especiales son:
- **include/driver.hpp:**  Esta clase proporciona una comunicacion con el codigo y el parser/lexer

- **source/parser.y:** Archivo con las reglas de la gramatica (Bison)

- **source/lexer.l:** Archivo con las reglas de los tokens (Flex)

- **source/driver.cpp:** Implementacion del driver

# Instalación
La compilacion de esta practica puede realizarse de la siguente forma:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Uso
Una vez se han procesado los anteriores comandos, el interprete se puede ejecutar
de la siguente forma:

`$ ./lambda`

A continuacion se muestran algunos ejemplos de comandos:

```
$ ./lambda
In [1]: #open ../examples.lm;;
```