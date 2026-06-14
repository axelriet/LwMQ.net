del /s /q .\out\*.*
rd /s /q .\out\*.*
sphinx-build -M html ./docs ./out
