
add_an_tool(Unsigned main.cpp)

target_link_libraries(Unsigned PRIVATE Basic libunsigned)

add_an_tool(UnsignedCipher main_cipher.cpp)
target_link_libraries(UnsignedCipher PRIVATE Basic libunsigned)