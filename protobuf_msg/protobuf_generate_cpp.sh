Dir_msg_proto=$1
Dir_msg_src=$2

echo "proto_msg header path : ${Dir_msg_proto}"
echo "proto_msg source output path : ${Dir_msg_src}"

protoc -I=${Dir_msg_proto} --cpp_out=${Dir_msg_src} ${Dir_msg_proto}/*.proto

