#include "NametagGlobals.h"

TypeHandle NametagGlobals::_type_handle;

NotifyCategoryDef(NametagGlobals, "");

//template class pmap<unsigned int, state_map_t>;

PT(MouseWatcher) NametagGlobals::m_mouse_watcher = nullptr;
PT(AudioSound) NametagGlobals::m_click_sound = nullptr;
PT(AudioSound) NametagGlobals::m_rollover_sound = nullptr;

NodePath& NametagGlobals::m_camera_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_arrow_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_card_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_card_balloon_3d_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_card_balloon_2d_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_thought_balloon_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_nametag_model_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_page_button_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_nodepath EMPTY_NODEPATH;

ChatBalloon* NametagGlobals::speech_balloon_2d = nullptr;
ChatBalloon* NametagGlobals::speech_balloon_3d = nullptr;
ChatBalloon* NametagGlobals::thought_balloon_2d = nullptr;
ChatBalloon* NametagGlobals::thought_balloon_3d = nullptr;

NametagGlobals* NametagGlobals::_global_ptr = nullptr;

VBase4 NametagGlobals::m_nametag_frame = VBase4(-0.5, 0.5, -0.5, 0.5);

buttons_map_t NametagGlobals::page_buttons;
buttons_map_t NametagGlobals::quit_buttons;

color_map_t NametagGlobals::nametag_colors;
whisper_color_map_t NametagGlobals::whisper_colors;

bool NametagGlobals::m_want_active_nametags = 1;
bool NametagGlobals::m_force_2d_nametags = 0;
bool NametagGlobals::m_force_onscreen_chat = 0;

NametagGlobals::NametagGlobals() { 
    NametagGlobals_cat.debug() << "__init__()" << std::endl;
}

NametagGlobals::~NametagGlobals() {
    delete m_click_sound;
    m_click_sound = nullptr;
    delete m_rollover_sound;
    m_rollover_sound = nullptr;
}

void NametagGlobals::add_color(unsigned int cc, LVecBase4f normal_name_fg, LVecBase4f normal_name_bg, LVecBase4f normal_chat_fg, LVecBase4f normal_chat_bg, LVecBase4f click_name_fg, LVecBase4f click_name_bg, LVecBase4f click_chat_fg, LVecBase4f click_chat_bg, LVecBase4f hover_name_fg, LVecBase4f hover_name_bg, LVecBase4f hover_chat_fg, LVecBase4f hover_chat_bg, LVecBase4f disabled_name_fg, LVecBase4f disabled_name_bg, LVecBase4f disabled_chat_fg, LVecBase4f disabled_chat_bg) {
    NametagGlobals_cat.debug() << "add_color() -- If you want to see all of these parameters. Add your own debug message!" << std::endl;
    color_tuple_t normal_name(2);
    normal_name[0] = normal_name_fg;
    normal_name[1] = normal_name_bg;
    
    color_tuple_t normal_chat(2);
    normal_chat[0] = normal_chat_fg;
    normal_chat[1] = normal_chat_bg;
    
    color_tuple_tuple_t normal(2);
    normal[0] = normal_name;
    normal[1] = normal_chat;
    
    color_tuple_t click_name(2);
    click_name[0] = click_name_fg;
    click_name[1] = click_name_bg;
    
    color_tuple_t click_chat(2);
    click_chat[0] = click_chat_fg;
    click_chat[1] = click_chat_bg;
    
    color_tuple_tuple_t click(2);
    click[0] = click_name;
    click[1] = click_chat;
    
    color_tuple_t hover_name(2);
    hover_name[0] = hover_name_fg;
    hover_name[1] = hover_name_bg;
    
    color_tuple_t hover_chat(2);
    hover_chat[0] = hover_chat_fg;
    hover_chat[1] = hover_chat_bg;
    
    color_tuple_tuple_t hover(2);
    hover[0] = hover_name;
    hover[1] = hover_chat;
    
    color_tuple_t disabled_name(2);
    disabled_name[0] = disabled_name_fg;
    disabled_name[1] = disabled_name_bg;
    
    color_tuple_t disabled_chat(2);
    disabled_chat[0] = disabled_chat_fg;
    disabled_chat[1] = disabled_chat_bg;
    
    color_tuple_tuple_t disabled(2);
    disabled[0] = disabled_name;
    disabled[1] = disabled_chat;
    
    state_map_t def;
    def[CLICKSTATE_NORMAL] = normal;
    def[CLICKSTATE_CLICK] = click;
    def[CLICKSTATE_HOVER] = hover;
    def[CLICKSTATE_DISABLED] = disabled;
    
    nametag_colors[cc] = def;
}

void NametagGlobals::add_whisper_color(unsigned int wt, LVecBase4f normal_fg, LVecBase4f normal_bg, LVecBase4f click_fg, LVecBase4f click_bg, LVecBase4f hover_fg, LVecBase4f hover_bg, LVecBase4f disabled_fg, LVecBase4f disabled_bg) {
    NametagGlobals_cat.debug() << "add_whisper_color() -- If you want to see all of these parameters. Add your own debug message!" << std::endl;
    color_tuple_t normal(2);
    normal[0] = normal_fg;
    normal[1] = normal_bg;
    
    color_tuple_t click(2);
    click[0] = click_fg;
    click[1] = click_bg;
    
    color_tuple_t hover(2);
    hover[0] = hover_fg;
    hover[1] = hover_bg;
    
    color_tuple_t disabled(2);
    disabled[0] = disabled_fg;
    disabled[1] = disabled_bg;
    
    state_map_simple_t def;
    def[CLICKSTATE_NORMAL] = normal;
    def[CLICKSTATE_CLICK] = click;
    def[CLICKSTATE_HOVER] = hover;
    def[CLICKSTATE_DISABLED] = disabled;
    
    whisper_colors[wt] = def;
}

void NametagGlobals::set_speech_balloon_3d(ChatBalloon* sb3d) {
    NametagGlobals_cat.debug() << "set_speech_balloon_3d(ChatBalloon ballon)" << std::endl;
    speech_balloon_3d = sb3d;
}

void NametagGlobals::set_thought_balloon_3d(ChatBalloon* tb3d) {
    NametagGlobals_cat.debug() << "set_thought_balloon_3d(ChatBalloon ballon)" << std::endl;
    thought_balloon_3d = tb3d;
}

void NametagGlobals::set_speech_balloon_2d(ChatBalloon* sb2d) {
    NametagGlobals_cat.debug() << "set_speech_balloon_2d(ChatBalloon ballon)" << std::endl;
    speech_balloon_2d = sb2d;
}

void NametagGlobals::set_thought_balloon_2d(ChatBalloon* tb2d) {
    NametagGlobals_cat.debug() << "set_thought_balloon_2d(ChatBalloon ballon)" << std::endl;
    thought_balloon_2d = tb2d;
}

void NametagGlobals::set_toon(NodePath& toon) {
    NametagGlobals_cat.debug() << "set_toon(NodePath toon)" << std::endl;
    m_nodepath = toon;
}

void NametagGlobals::set_arrow_model(NodePath& node) {
    NametagGlobals_cat.debug() << "set_arrow_model(NodePath node)" << std::endl;
    m_arrow_nodepath = node;
}

void NametagGlobals::set_card_model(NodePath& node) {
    NametagGlobals_cat.debug() << "set_card_model(NodePath node)" << std::endl;
    m_card_nodepath = node;
}

void NametagGlobals::set_nametag_card(NodePath& model, VBase4 frame) {
    NametagGlobals_cat.debug() << "set_nametag_card(NodePath model, VBase4 frame)" << std::endl;
    m_nametag_frame = frame;
    m_card_nodepath = model;
}

void NametagGlobals::set_mouse_watcher(MouseWatcher& watcher) {
    NametagGlobals_cat.debug() << "set_mouse_watcher(MouseWatcher watcher)" << std::endl;
    m_mouse_watcher = &watcher;
}

void NametagGlobals::set_mouse_watcher(NodePath& np) {
    NametagGlobals_cat.debug() << "set_mouse_watcher(NodePath np)" << std::endl;
    m_mouse_watcher = DCAST(MouseWatcher, np.node());
}

void NametagGlobals::set_camera(NodePath& node) {
    NametagGlobals_cat.debug() << "set_camera(NodePath node)" << std::endl;
    m_camera_nodepath = node;
}

void NametagGlobals::set_page_button(int state, NodePath& model) {
    NametagGlobals_cat.debug() << "set_page_button(" << state << " NodePath model)" << std::endl;
    page_buttons[state] = model;
}

void NametagGlobals::set_page_button(NodePath& model, NodePath& model1, NodePath& model2, NodePath& model3) {
    NametagGlobals_cat.debug() << "set_page_button(NodePath model, NodePath model1, NodePath model2, NodePath model3)" << std::endl;
    page_buttons[0] = model;
    page_buttons[1] = model1;
    page_buttons[2] = model2;
    page_buttons[3] = model3;
}

void NametagGlobals::set_quit_button(int state, NodePath& model) {
    NametagGlobals_cat.debug() << "set_quit_button(" << state << " NodePath model)" << std::endl;
    quit_buttons[state] = model;
}

void NametagGlobals::set_quit_button(NodePath& model, NodePath& model1, NodePath& model2, NodePath& model3) {
    NametagGlobals_cat.debug() << "set_quit_button(NodePath model, NodePath model1, NodePath model2, NodePath model3)" << std::endl;
    quit_buttons[0] = model;
    quit_buttons[1] = model1;
    quit_buttons[2] = model2;
    quit_buttons[3] = model3;
}

void NametagGlobals::set_want_active_nametags(bool want) {
    NametagGlobals_cat.debug() << "set_want_active_nametags(" << want << ")" << std::endl;
    m_want_active_nametags = want;
}

void NametagGlobals::set_force_onscreen_chat(bool want) {
    NametagGlobals_cat.debug() << "set_force_onscreen_chat(" << want << ")" << std::endl;
    m_force_onscreen_chat = want;
}

void NametagGlobals::set_force_2d_nametags(bool want) {
    NametagGlobals_cat.debug() << "set_force_2d_nametags(" << want << ")" << std::endl;
    m_force_2d_nametags = want;
}

void NametagGlobals::set_click_sound(PT(AudioSound) sound) {
    NametagGlobals_cat.debug() << "set_click_sound(AudioSound sound)" << std::endl;
    m_click_sound = sound;
}

void NametagGlobals::set_rollover_sound(PT(AudioSound) sound) {
    NametagGlobals_cat.debug() << "set_rollover_sound(AudioSound sound)" << std::endl;
    m_rollover_sound = sound;
}

PT(AudioSound) NametagGlobals::get_click_sound() {
    NametagGlobals_cat.debug() << "get_click_sound()" << std::endl;
    return m_click_sound;
}

PT(AudioSound) NametagGlobals::get_rollover_sound() {
    NametagGlobals_cat.debug() << "get_rollover_sound()" << std::endl;
    return m_rollover_sound;
}

NodePath& NametagGlobals::get_toon() {
    NametagGlobals_cat.debug() << "get_toon()" << std::endl;
    return m_nodepath;
}

NodePath& NametagGlobals::get_arrow_model() {
    NametagGlobals_cat.debug() << "get_arrow_model()" << std::endl;
    return m_arrow_nodepath;
}

NodePath& NametagGlobals::get_card_model() {
    NametagGlobals_cat.debug() << "get_card_model()" << std::endl;
    return m_card_nodepath;
}

NodePath& NametagGlobals::get_nametag_card() {
    NametagGlobals_cat.debug() << "get_nametag_card()" << std::endl;
    return m_card_nodepath;
}

NodePath& NametagGlobals::get_chat_balloon_3d_model() {
    NametagGlobals_cat.debug() << "get_chat_balloon_3d_model()" << std::endl;
    return m_card_balloon_3d_nodepath;;
}

NodePath& NametagGlobals::get_chat_balloon_2d_model() {
    NametagGlobals_cat.debug() << "get_chat_balloon_2d_model()" << std::endl;
    return m_card_balloon_2d_nodepath;;
}

NodePath& NametagGlobals::get_thought_balloon_model() {
    NametagGlobals_cat.debug() << "get_thought_balloon_model()" << std::endl;
    return m_thought_balloon_nodepath;
}

NodePath& NametagGlobals::get_camera() {
    NametagGlobals_cat.debug() << "get_camera()" << std::endl;
    return m_camera_nodepath;
}

VBase4 NametagGlobals::get_nametag_card_frame() {
    NametagGlobals_cat.debug() << "get_nametag_card_frame()" << std::endl;
    return m_nametag_frame;
}

LVecBase4f NametagGlobals::get_name_fg(unsigned int cc, int clickstate) {
    NametagGlobals_cat.debug() << "get_name_fg(" << cc << " " << clickstate << ")" << std::endl;
    
    if (clickstate < CLICKSTATE_NORMAL || clickstate > CLICKSTATE_DISABLED) {
        return LVecBase4f(1);
    }
    
    if (!does_color_code_exist(cc)) {
        return LVecBase4f(1);
    }
    
    return nametag_colors[cc][clickstate][0][0];
}

color_tuple_t NametagGlobals::get_whisper_colors(unsigned int wt, int clickstate) {
    NametagGlobals_cat.debug() << "get_whisper_colors(" << wt << " " << clickstate << ")" << std::endl;
    if (clickstate < CLICKSTATE_NORMAL || clickstate > CLICKSTATE_DISABLED) {
        color_tuple_t ret(2);
        ret[0] = LVecBase4f(1);
        ret[1] = LVecBase4f(1);
        return ret;
    }
    
    if (!does_whisper_type_exist(wt)) {
        color_tuple_t ret(2);
        ret[0] = LVecBase4f(1);
        ret[1] = LVecBase4f(1);
        return ret;
    }
    
    return whisper_colors[wt][clickstate];
}

bool NametagGlobals::get_want_active_nametags() {
    NametagGlobals_cat.debug() << "get_want_active_nametags()" << std::endl;
    return m_want_active_nametags;
}

bool NametagGlobals::get_force_onscreen_chat() {
    NametagGlobals_cat.debug() << "get_force_onscreen_chat()" << std::endl;
    return m_force_onscreen_chat;
}

bool NametagGlobals::get_force_2d_nametags() {
    NametagGlobals_cat.debug() << "get_force_2d_nametags()" << std::endl;
    return m_force_2d_nametags;
}

bool NametagGlobals::does_color_code_exist(unsigned int cc) {
    NametagGlobals_cat.debug() << "does_color_code_exist(" << cc << ")" << std::endl;
    return nametag_colors.count(cc) == 1;
}

bool NametagGlobals::does_whisper_type_exist(unsigned int wt) {
    NametagGlobals_cat.debug() << "does_whisper_type_exist(" << wt << ")" << std::endl;
    return whisper_colors.count(wt) == 1;
}

NametagGlobals* NametagGlobals::get_global_ptr() {
    NametagGlobals_cat.debug() << "get_global_ptr()" << std::endl;
    if ((_global_ptr == nullptr) || (_global_ptr == NULL)) {
        _global_ptr = new NametagGlobals;
    }
    return _global_ptr;
}

void* __initialize_constants() {
   NametagGlobals::add_color(NametagGlobals::CCNormal,
               (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.3, 0.3, 0.7, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCNoChat,
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (1.0, 0.5, 0.5, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (1.0, 0.5, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCNonPlayer,
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCSuit,
               (0.2, 0.2, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.2, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.4, 0.4, 0.4, 1.0), (1.0, 1.0, 1.0, 0.7),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCSuitBuilding,
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCToonBuilding,
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCHouseBuilding,
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.5, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCSpeedChat,
               (0.0, 0.6, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.5, 0.0, 1.0), (0.5, 0.5, 0.5, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.7, 0.2, 1.0), (1.0, 1.0, 1.0, 0.7),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_color(NametagGlobals::CCFreeChat,
               (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.5, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    NametagGlobals::add_whisper_color(NametagGlobals::WTNormal,
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.6, 0.8, 0.6),
                       (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.9, 0.6),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.8, 0.6)
    );

    NametagGlobals::add_whisper_color(NametagGlobals::WTQuickTalker,
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.6, 0.8, 0.6),
                       (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.9, 0.6),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.8, 0.6)
    );

    NametagGlobals::add_whisper_color(NametagGlobals::WTSystem,
                       (0.0, 0.0, 0.0, 1.0), (0.8, 0.3, 0.6, 0.6),
                       (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.8, 0.4, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.8, 0.3, 0.6, 0.6)
    );

// XXX TODO: WT_battle_SOS, WT_emote, WT_toontown_boarding_group
return nullptr;
}

void* _ = __initialize_constants();