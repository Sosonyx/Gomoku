NAME =  Gomoku

SRC =  	main.cpp  \
		board/Board.cpp  \
		game/Game.cpp	\
		game/Player.cpp	\
		render/Render.cpp	\
		analysis/Analyse.cpp	\
		analysis/LineView.cpp	\
		analysis/SideView.cpp	\
		search/Search.cpp	\
		search/SearchState.cpp	\

D_SRC = src/

D_BUILD = .build/
OBJ =  	$(addprefix $(D_BUILD), $(SRC:.cpp=.o))

CPP =  c++
FLAGS = -Wall -Wextra -Werror -MMD -O3 -g -std=c++23
INC = inc/
SDL_LIBS = sdl2 SDL2_ttf SDL2_image SDL2_gfx
IFLAGS =  $(addprefix -I, $(INC)) $(shell pkg-config --cflags $(SDL_LIBS))
LIB_FLAGS = $(shell pkg-config --libs $(SDL_LIBS))

RM =  rm -rf

MAKE += --no-print-directory

all:	$(NAME)

$(NAME):	 $(OBJ)
	$(CPP) -o$@ $(OBJ) $(LIB_FLAGS)

$(OBJ): $(D_BUILD)%.o:	$(D_SRC)%.cpp
	@mkdir -p $(@D)
	$(CPP) $(FLAGS) $(IFLAGS) -c $< -o $@

clean:
	@$(RM) $(D_BUILD)

fclean: clean
	@$(RM) $(NAME)

re: fclean
	@$(MAKE) all

DEPS = $(addprefix $(D_BUILD), $(SRC:.cpp=.d))
-include $(DEPS)

.PHONY: re fclean clean all
