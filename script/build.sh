#!/bin/bash

# We only make check, not changes
DOX_CONF_FILE=$(pwd)/Doxyfile
# Obtenemos la ruta absoluta del directorio del proyecto
PROJECT_PATH=$(pwd)
ERROR_FILE_FLAG=$PROJECT_PATH/clang-format_errors.txt

echo $PROJECT_PATH

SOURCE_FILES=$(find $PROJECT_PATH/src -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")
SOURCE_FILES+=$(find $PROJECT_PATH/include -type f \( -name "*.cpp" -or -name "*.hpp" -or -name "*.h" -or -name "*.c" \) | tr "\n" " ")

clang-format -i $SOURCE_FILES
clang-format -n $SOURCE_FILES 2> ${ERROR_FILE_FLAG}

# Ruta del directorio build
BUILD_DIR="build"

# Verificar si el directorio build existe
if [ -d "$BUILD_DIR" ]; then
    # Recorrer todos los archivos y carpetas dentro del directorio build
    for item in "$BUILD_DIR"/* "$BUILD_DIR"/.[!.]* "$BUILD_DIR"/..?*; do
        # Si el nombre del archivo o carpeta no es "_deps"
        if [ "$(basename "$item")" != "_deps" ]; then
            # Eliminar el archivo o carpeta
            rm -rf "$item"
        fi
    done
else
    echo "El directorio $BUILD_DIR no existe."
fi

# Crea el directorio build y entra en él
mkdir -p $BUILD_DIR && cd $BUILD_DIR

# Configura el proyecto usando CMake con el generador Ninja
if [ "$1" == "test" ]; then
    # cmake -GNinja -DRUNS_COVERAGE=1 -DRUNS_TESTS=1 -DCMAKE_BUILD_TYPE=Release ..
    cmake -GNinja -DRUNS_COVERAGE=1 -DRUNS_TESTS=1 ..
    ninja -j$(nproc)
    ctest --output-on-failure -VV
    gcovr -r .. --html --html-details -o reports/coverage.html
elif [ "$1" == "debug" ]; then
    cmake -GNinja -DCMAKE_BUILD_TYPE=Debug ..
    ninja -j$(nproc)
else
    cmake -GNinja ..
    ninja -j$(nproc)
fi

# Append to DOX_CONF_FILE input source directories 
{
    cat $DOX_CONF_FILE
    echo "INPUT" = $PROJECT_PATH/include
} > $DOX_CONF_FILE

# Generate documentation
# dot -c clears Graphviz configuration, doxygen uses Graphviz for generating graphical representations
sudo dot -c

ERROR_FILE_FLAG=$PROJECT_PATH/dox_errors.txt

# create documentation: -s specifies comments of configurations items will be omitted.
# pipe stderr to error file
DOXYGEN_COMMAND=$(doxygen -s $DOX_CONF_FILE 2> $ERROR_FILE_FLAG)

# if error file not empty fail
if [ -s $ERROR_FILE_FLAG ]; then
echo "Error: There are some files that are not documented correctly"
cat $ERROR_FILE_FLAG
exit 1
else
echo "All files are documented correctly. Niiiceee"
fi

# Encuentra todos los archivos de texto (puedes ajustar la extensión según sea necesario)
find $PROJECT_PATH -type f -name "*.txt" -o -name "*.md" -o -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" -o -name "*.sh" | while read file; do
  # Comprueba si el archivo no termina con una nueva línea
  if [ "$(tail -c1 "$file" | wc -l)" -ne 1 ]; then
    # Añade una nueva línea al final del archivo
    echo >> "$file"
    echo "Added newline to the end of: $file"
  fi
done

exit 0
