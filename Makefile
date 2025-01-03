# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 15:48:20 by amakinen          #+#    #+#              #
#    Updated: 2024/12/19 21:53:10 by amakinen         ###   ########.fr        #
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
ifdef DEBUG
	MLX42_FLAGS = -DDEBUG=1
else
	MLX42_FLAGS = -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=1
endif
$(MLX42): mlx42-make
	cmake lib/MLX42 -B lib/MLX42/build $(MLX42_FLAGS)
	cmake --build lib/MLX42/build
mlx42-clean:
	rm -rf lib/MLX42/build
clean: mlx42-clean
.PHONY: mlx42-clean mlx42-make

# Optimization flags
OPT_FLAGS := -flto -O3

# Project files and targets
SRCS := $(addprefix $(SRCDIR)/,\
	main.c \
	fdf.c \
	str_util.c \
	color.c \
	line/line.c \
	mesh.c \
	buffer.c \
	file_reader.c \
	map/map_builder.c \
	map/map_builder_add_entry.c \
	map/map_load.c \
	parse_int.c \
	vec_mat/vec.c \
	vec_mat/vec_maxmin.c \
	vec_mat/mat.c \
	vec_mat/vec_mat_helper.c \
	rotation.c \
	camera/camera_control.c \
	camera/camera_transform.c \
	input/input_timed.c \
	input/input_event.c \
	z_image.c \
)

OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BINS := $(NAME)
$(NAME): $(OBJS) $(MLX42)
$(NAME): tgt_LDLIBS += -lm $(MLX42LIBS)

# Bonus is built in, no separate bonus code or program
BONUS := $(NAME)

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
def_CFLAGS := -Wall -Wextra -Werror -g
def_CPPFLAGS := -MMD -MP $(addprefix -I ,$(INCDIRS))

# Add sanitizer flags if requested
ifneq (,$(strip $(SANITIZE)))
	def_CFLAGS += -fsanitize=$(SANITIZE)
	def_LDFLAGS += -fsanitize=$(SANITIZE)
endif

# Add optimization flags unless debug requested
ifndef DEBUG
	def_CFLAGS += $(OPT_FLAGS)
	def_LDFLAGS += $(OPT_FLAGS)
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
