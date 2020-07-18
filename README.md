# Introducción
Este proyecto ha sido la práctica de la asignatura de diseño de lenguajes de programación, en la Universidad de La Coruña. Implementamos un intérprete de *lambda cálculo* usando el lenguaje de programación C++. Este intérprete soporta tipos así como un subconjunto de funcionalidades del *lambda cálculo* tipado.

Algunas de estas funcionalidades son:

- Naturales
- Strings
- Floats
- Bools
- Units
- Listas y concatenaciones
- Tuplas y acceso mediante indices
- Registros y acceso mediante etiqueta
- Condiciones if-then-else
- Simple pattern-matching
- Recursividad
- Alias
- Ascripciones
 
Algunas limitaciones de esta práctica a tener en cuenta son:

- Cada línea del terminal lee hasta que encuentra un salto de línea, esta limitación viene dada por el interprete.
- Las aplicaciones (por ejemplo: id 1) no soportan el estilo *currificado*,
      esto significa que solo podemos aplicar un término a la vez, 
      para usar mas terminos necesitamos usar paréntesis. Esta limitacion viene impuesta por el parser y se ha decidido 
      no soportar esta funcionalidad con el fin de evitar problemas de *shift/reduce*.
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
de la siguente manera:

`$ ./lambda`
 
Se pueden leer ficheros externos con código mediante la operación:
 ```#open ruta_del_archivo;;```

El archivo *examples.lm* contine ejemplos del lenguaje, que pueden ser ejecutados de la siguiente manera:
```
$ ./lambda
In [1]: #open ../examples.lm;;
```
