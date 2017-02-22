#!/bin/bash
CommandUpperCase=${1^^}
CommandCapitalized=${1^}
TargetUpperCase=${2^^}
cp ${1}_builder_template.hpp __temp_builder__ \
&& (  \
  cat __temp_builder__ \
  | sed -- "s/__CommandType__/${CommandCapitalized}/g" \
  | sed -- "s/__ObjectType__/${3}/g" \
  | sed -- "s/__objectType__/${2,}/g" \
  | sed -- "s/__objtype__/${3,,}/g" \
  > $1/$1_$2_builder.hpp) && rm __temp_builder__ && ( \
  echo "/*"; cat $IROHA_HOME/LICENSE; echo "*/";
  echo "#ifndef CORE_MODEL_TRANSACTION_BUILDER_${CommandUpperCase}_${TargetUpperCase}_HPP";
  echo "#define CORE_MODEL_TRANSACTION_BUILDER_${CommandUpperCase}_${TargetUpperCase}_HPP";
  echo "";
  echo "#include <infra/protobuf/api.pb.h>";
  echo "#include <util/exception.hpp>";
  echo "#include \"../transaction_builder_base.hpp\"";
  echo "#include \"../type_signatures/commands/$1.hpp\"";
  echo "#include \"../type_signatures/objects.hpp\"";
  echo "";
  g++ -E $1/$1_$2_builder.hpp \
    | clang-format -style=Google \
    | sed -e '1,7d'; \
  echo "";
  echo "#endif"
) > $1/$1_$2_builder_temp.hpp \
  && mv $1/$1_$2_builder_temp.hpp $1/$1_$2_builder.hpp