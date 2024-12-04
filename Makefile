# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 15:48:20 by amakinen          #+#    #+#              #
#    Updated: 2024/12/04 18:11:15 by amakinen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := fdf

# Directories
OBJDIR := obj
SRCDIR := src
INCDIRS := include

# MLX42
MLX42 := lib/MLX42/build/libmlx42.a
INCDIRS += lib/MLX42/include
MLX42LIBS := -lglfw
$(MLX42): mlx42-make
	cmake lib/MLX42 -B lib/MLX42/build -DDEBUG=1
	cmake --build lib/MLX42/build
mlx42-clean:
	rm -rf lib/MLX42/build
clean: mlx42-clean
.PHONY: mlx42-clean mlx42-make

# Project files and targets
SRCS := $(addprefix $(SRCDIR)/,\
	main.c \
	color.c \
	line.c \
	mesh.c \
	buffer.c \
	file_reader.c \
	map/map_builder.c \
	map/map_builder_add_entry.c \
	map/map_load.c \
	parse_int.c \
	vec_mat/vec.c \
	vec_mat/mat.c \
	vec_mat/vec_mat_helper.c \
)

OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BINS := $(NAME)
$(NAME): $(OBJS) $(MLX42)
$(NAME): tgt_LDLIBS += -lm $(MLX42LIBS)

# Generic utility targets
.DEFAULT_GOAL := all

.PHONY: all bonus clean fclean re

all: $(NAME)

bonus: $(BONUS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(BINS)

re: fclean all

# Default compiler flags that apply to all targets
def_CFLAGS := -Wall -Wextra -Werror
def_CPPFLAGS := -MMD -MP $(addprefix -I ,$(INCDIRS))

# Add sanitizer flags if requested
ifneq (,$(strip $(SANITIZE)))
	def_CFLAGS += -g -fsanitize=$(SANITIZE)
	def_LDFLAGS += -g -fsanitize=$(SANITIZE)
endif

# Combine default def_FLAGS, target specific tgt_FLAGS and user-supplied FLAGS
# into one _FLAGS variable to be used in recipes
flagvars = CFLAGS CPPFLAGS LDFLAGS LDLIBS
$(foreach v,$(flagvars),$(eval _$v = $$(strip $$(def_$v) $$(tgt_$v) $$($v))))

# Recipe command to ensure directory for target exists
mktargetdir = @mkdir -p $(@D)

# Default recipes for each type of target
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(mktargetdir)
	$(CC) $(_CPPFLAGS) $(_CFLAGS) -c $< -o $@

$(BINS):
	$(mktargetdir)
	$(CC) $(_LDFLAGS) $^ $(_LDLIBS) -o $@

# Inform make that object files don't need to be remade if the requested
# targets are up to date with respect to the source files.
.SECONDARY: $(OBJS)

# Dependency files to handle #include dependencies
DEPS = $(OBJS:.o=.d)
-include $(DEPS)
