NULL=

RETRO_DIR = retro
FLICKY_DIR = flicky
DEMO_DIR = demo

OUT_DIR = out
VAPI_DIR = vapi

DEMO = $(OUT_DIR)/demo

RETRO_FILES= \
	Audio.vala \
	Camera.vala \
	Core.vala \
	Device.vala \
	Disk.vala \
	Environment.vala \
	FrameTime.vala \
	GameInfo.vala \
	GameType.vala \
	Hardware.vala \
	Keyboard.vala \
	Log.vala \
	Memory.vala \
	Message.vala \
	Performance.vala \
	Region.vala \
	Rumble.vala \
	Sensor.vala \
	SystemAvInfo.vala \
	SystemInfo.vala \
	Variable.vala \
	Video.vala \
	retro-core-extern.c \
	retro-environment-extern.c \
	$(NULL)

FLICKY_FILES= \
	AudioSamples.vala \
	Options.vala \
	Runnable.vala \
	Runner.vala \
	$(NULL)

DEMO_FILES= \
	Demo.vala \
	Engine.vala \
	Window.vala \
	OptionsDialog.vala \
	AudioDevice.vala \
	ControllerDevice.vala \
	KeyboardHandler.vala \
	lol.c \
	$(NULL)

RETRO_PKG= \
	gmodule-2.0 \
	stdint \
	$(NULL)

FLICKY_PKG= \
	$(NULL)

PKG= \
	$(RETRO_PKG) \
	$(FLICKY_PKG) \
	gtk+-3.0 \
	libpulse \
	libpulse-mainloop-glib \
	$(NULL)

RETRO_LIBNAME=retro
FLICKY_LIBNAME=flicky

RETRO_SRC = $(RETRO_FILES:%=$(RETRO_DIR)/%)
FLICKY_SRC = $(FLICKY_FILES:%=$(FLICKY_DIR)/%)
DEMO_SRC = $(DEMO_FILES:%=$(DEMO_DIR)/%)

RETRO_OUT=$(OUT_DIR)/lib$(RETRO_LIBNAME).so
FLICKY_OUT=$(OUT_DIR)/lib$(FLICKY_LIBNAME).so

all: $(DEMO)

$(DEMO): $(RETRO_SRC) $(FLICKY_SRC) $(DEMO_SRC)
	mkdir -p $(OUT_DIR)
	valac -b $(RETRO_DIR) -d $(@D) \
		-o $(@F) $^ \
		--vapidir=$(VAPI_DIR) $(PKG:%=--pkg=%) \
		--save-temps \
		-g

$(RETRO_OUT): $(RETRO_SRC)
	mkdir -p $(@D)
	echo $(RETRO_PKG) > $(@D)/$(RETRO_LIBNAME).deps
	valac \
		-b $(<D) -d $(@D) \
		--library=$(RETRO_LIBNAME) \
		--vapi=$(RETRO_LIBNAME)-1.0.vapi \
		--gir=$(RETRO_LIBNAME)-1.0.gir \
		-H $(@D)/$(RETRO_LIBNAME).h \
		-o $(@F) $^ \
		--vapidir=$(VAPI_DIR) $(RETRO_PKG:%=--pkg=%) \
		--save-temps \
		-X -fPIC -X -shared

$(FLICKY_OUT): $(FLICKY_SRC)
	mkdir -p $(@D)
	echo $(FLICKY_PKG) > $(@D)/$(FLICKY_LIBNAME).deps
	valac \
		-b $(<D) -d $(@D) \
		--library=$(FLICKY_LIBNAME) \
		--vapi=$(FLICKY_LIBNAME)-1.0.vapi \
		--gir=$(FLICKY_LIBNAME)-1.0.gir \
		-H $(@D)/$(FLICKY_LIBNAME).h \
		-o $(@F) $^ \
		--vapidir=$(VAPI_DIR) $(FLICKY_PKG:%=--pkg=%) \
		--save-temps \
		-X -fPIC -X -shared

clean:
	rm -Rf $(OUT_DIR)

.PHONY: all test clean

