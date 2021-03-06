/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 16;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 4;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font Mono:size=14:antialias=true:autohint=true", "FontAwesome:size=14:antialias=true:autohint=true", "Symbola:size=14:antialias=true:autohint=true", "monospace:size=14" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font Mono:size=14:antialias=true:autohint=true";

/* gruvbox colors https://github.com/morhetz/gruvbox#dark-mode-1 */
#define COLOR_235_BG     "#282828"
#define COLOR_124_RED    "#cc241d"
#define COLOR_106_GREEN  "#98971a"
#define COLOR_172_YELLOW "#d79921"
#define COLOR_066_BLUE   "#458588"
#define COLOR_132_PURPLE "#b16286"
#define COLOR_072_AQUA   "#689d6a"
#define COLOR_246_GRAY   "#a89984"
#define COLOR_245_GRAY   "#928374"
#define COLOR_167_RED    "#fb4934"
#define COLOR_142_GREEN  "#b8bb26"
#define COLOR_214_YELLOW "#fabd2f"
#define COLOR_109_BLUE   "#83a598"
#define COLOR_175_PURPLE "#d3869b"
#define COLOR_108_AQUA   "#8ec07c"
#define COLOR_223_FG     "#ebdbb2"
#define COLOR_234_BG0_H  "#1d2021"
#define COLOR_235_BG0    "#282828"
#define COLOR_237_BG1    "#3c3836"
#define COLOR_239_BG2    "#504945"
#define COLOR_241_BG3    "#665c54"
#define COLOR_243_BG4    "#7c6f64"
#define COLOR_166_ORANGE "#d65d0e"
#define COLOR_236_BG0_S  "#32302f"
#define COLOR_246_FG4    "#a89984"
#define COLOR_248_FG3    "#bdae93"
#define COLOR_250_FG2    "#d5c4a1"
#define COLOR_223_FG1    "#ebdbb2"
#define COLOR_229_FG0    "#fbf1c7"
#define COLOR_208_ORANGE "#fe8019"

static const char col_norm_bg[] = COLOR_235_BG0;
static const char col_norm_fg[] = COLOR_246_FG4;
static const char col_norm_br[] = COLOR_237_BG1;
static const char col_selc_bg[] = COLOR_234_BG0_H;
static const char col_selc_fg[] = COLOR_229_FG0;
static const char col_selc_br[] = COLOR_246_FG4;

static const char *colors[][3]      = {
	/*               fg           bg           border   */
	[SchemeNorm] = { col_norm_fg, col_norm_bg, col_norm_br },
	[SchemeSel]  = { col_selc_fg, col_selc_bg, col_selc_br },
};

/* staticstatus */
static const int statmonval = 0;

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class             instance         title        tags mask     isfloating   monitor */
	{ "Google-chrome",   NULL,            NULL,        0,            0,           -1 },
	{ "Blueman-manager", NULL,            NULL,        1 << 8,       1,           -1 },
	{ "SpeedCrunch",     NULL,            NULL,        1 << 8,       1,           -1 },
	{ "flameshot",       NULL,            NULL,        1 << 8,       1,           -1 },
	{ "flameshot",       NULL,            NULL,        1 << 8,       1,           -1 },
	{ "zoom",           "zoom", "Participants",        0,            1,           -1 },
	{ "zoom",           "zoom", "Zoom - Free Account", 0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_norm_bg, "-nf", col_norm_fg, "-sb", col_selc_bg, "-sf", col_selc_fg, NULL };
static const char *termcmd[]  = { "x-terminal-emulator", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_Left,   viewtoleft,     {0} },
	{ MODKEY,                       XK_Right,  viewtoright,    {0} },
	{ MODKEY|ShiftMask,             XK_Left,   tagtoleft,      {0} },
	{ MODKEY|ShiftMask,             XK_Right,  tagtoright,     {0} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY|ControlMask|ShiftMask, XK_equal,  setgaps,        {.i = gappx  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

