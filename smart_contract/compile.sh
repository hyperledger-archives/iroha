rm *.so

# Account Repository
javac -encoding UTF-8 repository/AccountRepository.java
javah -jni repository.AccountRepository

# Asset Repository
javac -encoding UTF-8 repository/AssetRepository.java
javah -jni repository.AssetRepository

# For Test
cd Test; ./compile.sh
