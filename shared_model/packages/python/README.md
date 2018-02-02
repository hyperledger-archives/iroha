# Python client library example

## How to install iroha python library
If you want latest release:
```pip install iroha
```
If you want developer version:
```pip install -i https://testpypi.python.org/pypi iroha
```

## How to build iroha
Creating MacOS wheel and publishing it:
```python setup.py bdist_wheel
twine upload --repository-url https://pypi.org/legacy/ dist/*
```
Creating Linux source distribution and publishing it:
```python setup.py sdist
twine upload --repository-url https://pypi.org/legacy/ dist/*
```
