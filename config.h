/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Mononoki Nerd Font:size=16" };
static const char dmenufont[]       = "Mononoki:size=16";
static const char col_gray1[]       = "#282828";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#928374";
static const char col_gray4[]       = "#fbf1c7";
static const char col_orange[]      = "#F08E33";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    /* [SchemeSel]  = { col_gray4, col_orange,  col_orange  }, */
    [SchemeSel]  = { col_gray1, col_gray4,  col_gray4  },
    [SchemeHid]  = { col_orange,  col_gray1, col_orange  },
};

/* tagging */
/* static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; */
/* static const char *tags[] = {"", "", "", "", "", "", "", "", ""}; */
/* static const char *tags[] = {"", "", "", "", "", "", "🤓", "", ""}; */
static const char *tags[] = {"", "", "", "", "", "", "", "", ""};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "gimp",     NULL,       NULL,       0,            1,           -1 },
    { "firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
    { "discord",  NULL,       NULL,       1 << 7,       0,           -1 },
    { "mpv",      NULL,       NULL,    ~( 1 | ( 1 << 7 ) ), 0,           -1 },
    /* { "St",       NULL,       NULL,       (1 << 8) - 1, 0,           -1 }, */
    { "Steam",    NULL,       NULL,       1 << 3,       0,           -1 },
    { "Emacs",    NULL,       NULL,       1 | ( 1 << 8 ), 0,           -1 },
    { "Spotify",  NULL,       NULL,       1 << 2,       0,           -1 },
};

/* layout(s) */
/* static const float mfact     = 0.333333; /\* factor of master area size [0.05..0.95] *\/ */
static const float mfact     = 0.41; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "[@]",      spiral },
    { "[\\]",     dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_orange, "-sf", col_gray4, NULL };
static const char *rofidesk[]= { "rofi", "-show", "drun", NULL };
static const char *roficmd[] = { "rofi", "-show", "run",  NULL };
static const char *rofiwin[] = { "rofi", "-show", "window", NULL };
static const char *termcmd[] = { "st", "-e", "tmux", NULL };
static const char *termcon[] = { "st", "-e", "tmux", "a", NULL };
static const char *pcemacs[] = { "st", "-e", "ssh", "-tXYC", "home", "-p", "443", "tmux", "new", "-As", "emacs-connection", "emacsclient", "-c", NULL };
/* static const char *lockcmd[] = { "xsecurelock", NULL }; */
static const char *lockcmd[] = { "loginctl", "lock-session", NULL };
static const char *rotatecmd[] = { "screen_rotation.sh", NULL };
static const char *flipcmd[] = { "screen_flip.sh", NULL };
static const char *volup[] = { "pamixer", "-i", "5", NULL };
static const char *voldown[] = { "pamixer", "-d", "5", NULL };
static const char *mute[] = { "pamixer", "-t", NULL };
static const char *micmute[] = { "pamixer", "--default-source", "-t", NULL };

static const char *emacs[] = { "emacsclient", "-c", NULL };
static const char *emacsrestart[] = { "emacs_restart.sh", NULL };
static const char *emacsstart[] = { "emacs_start.sh", NULL };
static const char *emacsstop[] = { "emacs_stop.sh", NULL };

static const char *pausecmd[] = { "playerctl", "play-pause", NULL };
static const char *next[] = { "playerctl", "next", NULL };
static const char *prev[] = { "playerctl", "previous", NULL };
static const char *stop[] = { "playerctl", "stop", NULL };

static Key keys[] = {
    /* modifier                     key        function        argument */
    { 0,                            XF86XK_ScreenSaver, spawn, {.v = lockcmd } },
    { 0,                            XF86XK_RotateWindows, spawn, {.v = rotatecmd } },
    { 0,                            XF86XK_TaskPane, spawn, {.v = flipcmd } },
    { 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = volup } },
    { 0,                            XF86XK_AudioLowerVolume, spawn, {.v = voldown } },
    { 0,                            XF86XK_AudioMute,   spawn, {.v = mute } },
    { 0,                            XF86XK_AudioMicMute, spawn, {.v = micmute } },
    { 0,                            XF86XK_AudioPlay,   spawn, {.v = pausecmd } },
    { 0,                            XF86XK_AudioNext,   spawn, {.v = next } },
    { 0,                            XF86XK_AudioPrev,   spawn, {.v = prev } },
    { 0,                            XF86XK_AudioStop,   spawn, {.v = stop } },
    { MODKEY,                       XF86XK_Launch1,     spawn, {.v = emacsrestart } },
    { MODKEY|ControlMask,           XF86XK_Launch1,     spawn, {.v = emacsstop } },
    { MODKEY|ShiftMask,             XF86XK_Launch1,     spawn, {.v = emacsstart } },
    { ShiftMask,                    XF86XK_Launch1,     spawn, {.v = pcemacs } },
    { 0,                            XF86XK_Launch1,     spawn, {.v = emacs } },
    { MODKEY,                       XK_p,      spawn,          {.v = rofidesk } },
    { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = roficmd } },
    { MODKEY,                       XK_w,      spawn,          {.v = rofiwin } },
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    { MODKEY|ControlMask,           XK_Return, spawn,          {.v = termcon } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
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
    { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_w,      togglewin,      {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    TAGKEYS(                        XK_1,                      0 )
    TAGKEYS(                        XK_2,                      1 )
    TAGKEYS(                        XK_3,                      2 )
    TAGKEYS(                        XK_4,                      3 )
    TAGKEYS(                        XK_5,                      4 )
    TAGKEYS(                        XK_6,                      5 )
    TAGKEYS(                        XK_7,                      6 )
    TAGKEYS(                        XK_8,                      7 )
    TAGKEYS(                        XK_9,                      8 )
    {
        MODKEY|ShiftMask,             XK_q,      quit,           {0}
    },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button1,        togglewin,      {0} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    /* { ClkStatusText,        0,              Button1,        togglebar,      {0} }, */
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
