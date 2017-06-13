
# ToDo use macro
execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto block.proto")
execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto asset.proto")
execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto commands.proto")
execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto primitive.proto")
