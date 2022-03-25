if [ -e ./src/main.cpp ]
then
  mv ./src/main.cpp ./src/main
  mv ./src/mainForFileBased ./src/mainForFileBased.cpp
else
  mv ./src/main ./src/main.cpp
  mv ./src/mainForFileBased.cpp ./src/mainForFileBased
fi