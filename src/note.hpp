#include "plugin.hpp"
#include <chrono>


struct Note : TransparentWidget {
    // the note text
    std::string text = "Hello World.";

    // the margin between the text and the note border
    Vec margin = Vec(15.f, 15.f);
    // the note font size
    int font_size = 20;
    // the note alpha transparency
    float alpha = 1.f;
    // the note background color
    NVGcolor background_color = nvgRGBA(0x00, 0x00, 0x00, alpha * 0xff);
    // the note border color
    NVGcolor border_color = nvgRGBA(0x00, 0x00, 0x00, alpha * 0xff);
    // the note border radius
    float border_radius = 5.f;
    // the note border width
    float border_width = 1.f;
    // the note text color
    NVGcolor text_color = nvgRGBA(0xff, 0xff, 0xff, alpha * 0xff);
    // the time when the note was created
    std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
    // the duration the note should be displayed in seconds
    float duration = 2.f;
    
    // the note constructor
    Note() {
        this->box.size.x = getTextWidth() + margin.x * 2.f;
        this->box.size.y = getTextHeight() + margin.y * 2.f;
    }

    // the note destructor
    ~Note() {
        // remove the note from the rack
        if (this->parent != NULL) {
            this->parent->removeChild(this);
        }
    }

    // the note draw method
    void draw(const DrawArgs& args) override {
        // Fill the widget with the background color and transparency based on box.pos and box.size
        nvgBeginPath(args.vg);
        nvgRoundedRect(args.vg, 0.f, 0.f, this->box.size.x, this->box.size.y, this->border_radius);
        nvgFillColor(args.vg, this->background_color);
        nvgFill(args.vg);


        // Draw the border
        nvgStrokeColor(args.vg, this->border_color);
        nvgStrokeWidth(args.vg, this->border_width);
        nvgRoundedRect(args.vg, 0.f, 0.f, this->box.size.x, this->box.size.y, this->border_radius);
        nvgStroke(args.vg);

        // set the note font size
        nvgFontSize(args.vg, this->font_size);
        // set the note font face
        nvgFontFaceId(args.vg, APP->window->uiFont->handle);
        // set the note text color
        nvgFillColor(args.vg, this->text_color);
        // set the note text align
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        // set the note text
        nvgText(args.vg, this->box.size.x / 2.f, this->box.size.y / 2.f, this->text.c_str(), NULL);
    }

    // the note step method
    void step() override {
        // // get the sample time
        float sample_time = APP->engine->getSampleTime();
        // decrease the note alpha transparency
        this->alpha = std::max(0.0f, this->alpha - sample_time * 5000.f / duration);
        // move the note down slowly as it fades out
        box.pos.y += sample_time * 100000.f;
        // set the note background color
        this->background_color = nvgRGBA(0x00, 0x00, 0x00, alpha * 0xff);
        // set the note border color
        this->border_color = nvgRGBA(0x00, 0x00, 0x00, alpha * 0xff);
        // set the note text color
        this->text_color = nvgRGBA(0xff, 0xff, 0xff, alpha * 0xff);
        // check if the note should be removed
        if (this->alpha <= 0.f) {
            requestDelete();
        }
    }

    // get the width of the text
    float getTextWidth() {
        // get the note context
        NVGcontext* vg = APP->window->vg;
        // set the note font size
        nvgFontSize(vg, this->font_size);
        // get the width of the text
        float text_width = nvgTextBounds(vg, 0.f, 0.f, this->text.c_str(), NULL, NULL);
        // return the width of the text
        return text_width;
    }

    // get the height of the text
    float getTextHeight() {
        return this->font_size * 1.5f;
    }
};