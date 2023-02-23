#include "plugin.hpp"
#include <chrono>
#include "note.hpp"


struct Notiphy : Module {
	enum ParamId {
		TOGGLE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Notiphy() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(TOGGLE_PARAM, 0.f, 1.f, 0.f, "");
	}

	// a schmitt trigger to toggle the notification
	dsp::SchmittTrigger toggle_trigger;

	void process(const ProcessArgs& args) override {
		// check if the toggle button has been pressed
		if (toggle_trigger.process(params[TOGGLE_PARAM].getValue())) {
			// create a note
			Note* note = new Note();
			// set the note position to the bottom right corner
			note->box.pos.x = APP->scene->box.size.x - note->box.size.x - note->margin.x;
			note->box.pos.y = APP->scene->box.size.y - note->box.size.y - note->margin.y;
			// add the note to the rack
			APP->scene->addChild(note);
		}
	}
};


struct NotiphyWidget : ModuleWidget {
	NotiphyWidget(Notiphy* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/notiphy.svg")));

		// addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		// addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		// addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		// addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<LEDButton>(mm2px(Vec(10.16, 109.998)), module, Notiphy::TOGGLE_PARAM));
	}
};


Model* modelNotiphy = createModel<Notiphy, NotiphyWidget>("notiphy");