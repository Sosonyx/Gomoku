NAME =  Gomoku

SRC =  main.cpp
D_SRC = src/

D_BUILD = .build/
OBJ =  	$(addprefix $(D_BUILD), $(SRC:.cpp=.o))

CPP =  cpp
FLAGS = -Wall -Wextra -Werror -MMD -g
INC = inc/
IFLAGS =  $(addprefix -I, $(INC))
LIB_FLAGS = 

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
