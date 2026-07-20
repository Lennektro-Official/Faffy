.PHONY: install-dependencies build install export

# figure out linux distro and specify install prefix
DISTRO_ID := $(shell grep -oP '^ID=\K.*' /etc/os-release | tr -d '"')
PREFIX ?= /usr/local

# install the required dependencies based on the distro
install-dependencies:
	@echo "Installing dependencies for $(DISTRO_ID)..."
	@if [ "$(DISTRO_ID)" = "ubuntu" ] || [ "$(DISTRO_ID)" = "debian" ] || [ "$(DISTRO_ID)" = "linuxmint" ] || [ "$(DISTRO_ID)" = "pop" ]; then \
		sudo apt update && sudo apt install -y libgtk-3-dev libvte-2.91-dev; \
	elif [ "$(DISTRO_ID)" = "arch" ] || [ "$(DISTRO_ID)" = "manjaro" ]; then \
		sudo pacman -Syu --needed gtk3 vte3; \
	elif [ "$(DISTRO_ID)" = "fedora" ] || [ "$(DISTRO_ID)" = "rhel" ] || [ "$(DISTRO_ID)" = "centos" ]; then \
		sudo dnf install -y gtk3-devel vte291-devel; \
	elif [ "$(DISTRO_ID)" = "opensuse" ] || [ "$(DISTRO_ID)" = "opensuse-tumbleweed" ]; then \
		sudo zypper in -y gtk3-devel vte-devel; \
	else \
		echo "Could not detect distro. Please manually install the GTK3 and VTE-2.91 development headers."; \
		exit 1; \
	fi

# build the executable
build:
	gcc -O3 -s -fdata-sections -ffunction-sections -Wl,--gc-sections \
	faffy.c -o faffy `pkg-config --cflags --libs gtk+-3.0 vte-2.91`

# install the executable and desktop file to the system
install:
	@if [ ! -f faffy ]; then \
		echo "No executable to install present!"; \
		exit 1; \
	fi
	install -Dm755 faffy $(DESTDIR)$(PREFIX)/bin/faffy
	install -Dm644 faffy.desktop $(DESTDIR)$(PREFIX)/share/applications/faffy.desktop
	@echo " ==> Install successfull!"

# export is just a shorthand for build + install
export: build install
