NAME     := Gomoku

CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -Werror

SFML_TAG  := 2.6.2
SFML_DIR  := lib/SFML
SFML_SRC  := $(SFML_DIR)/src
SFML_BLD  := $(SFML_DIR)/build
SFML_INST := $(CURDIR)/$(SFML_DIR)/install
SFML_LIB  := $(SFML_INST)/lib/libsfml-graphics.so

CPPFLAGS := -I$(SFML_INST)/include
LDFLAGS  := -L$(SFML_INST)/lib -Wl,-rpath,$(SFML_INST)/lib
LDLIBS   := -lsfml-graphics -lsfml-window -lsfml-system

SRCS := src/main.cpp
OBJS := $(SRCS:src/%.cpp=obj/%.o)

all: $(NAME)

$(NAME): $(SFML_LIB) $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

obj/%.o: src/%.cpp | $(SFML_LIB)
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# SFML : clonée et compilée une seule fois (sans audio ni réseau).
$(SFML_LIB):
	@test -d $(SFML_SRC) || git clone --depth 1 --branch $(SFML_TAG) https://github.com/SFML/SFML.git $(SFML_SRC)
	cmake -S $(SFML_SRC) -B $(SFML_BLD) -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX=$(SFML_INST) -DBUILD_SHARED_LIBS=ON \
		-DSFML_BUILD_AUDIO=OFF -DSFML_BUILD_NETWORK=OFF \
		-DSFML_BUILD_EXAMPLES=OFF -DSFML_BUILD_DOC=OFF
	cmake --build $(SFML_BLD) -j
	cmake --install $(SFML_BLD)

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)
	rm -rf $(SFML_DIR)

re: fclean all

.PHONY: all clean fclean re
