Dir_lib=$1
Dir_protoc=$2
Dir_msg_proto=$3
Dir_msg_src=$4

echo "protoc used lib path: ${Dir_lib}"
echo "protoc path : ${Dir_protoc}"
echo "proto_msg header path : ${Dir_msg_proto}"
echo "proto_msg source output path : ${Dir_msg_src}"

export LD_LIBRARY_PATH=${Dir_lib}
${Dir_protoc}/protoc -I=${Dir_msg_proto} --cpp_out=${Dir_msg_src} ${Dir_msg_proto}/*.proto

