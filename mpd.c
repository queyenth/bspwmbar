#include <mpd/connection.h>
#include <mpd/player.h>
#include <mpd/song.h>
#include <mpd/response.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "bspwmbar.h"
#include "util.h"

static struct mpd_connection *conn;
static char song_name[512];

/* functions */
static void mpd_connect();
static void mpd_disconnect();
static const char* get_tag(const struct mpd_song*, enum mpd_tag_type);

static void
mpd_connect()
{
	conn = mpd_connection_new(NULL, 0, 0);

	if (conn == NULL) {
		die("Out of memory\n");
	}

	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		mpd_disconnect();
	}
}

static void
mpd_disconnect()
{
	mpd_connection_free(conn);
}

static const char*
get_tag(const struct mpd_song *song, enum mpd_tag_type type)
{
	unsigned i = 0;
	const char *value;

	while ((value = mpd_song_get_tag(song, type, i++)) != NULL) {
		return value;
	}

	return "";
}

void mpd(draw_context_t *dc, module_option_t *opts)
{
	if (!conn) {
		mpd_connect();
	}

	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		return;
	}

	if (mpd_send_current_song(conn)) {
		struct mpd_song *song = mpd_recv_song(conn);

		if (song != NULL) {
			sprintf(song_name, "%s - %s", get_tag(song, MPD_TAG_ARTIST),
						      get_tag(song, MPD_TAG_TITLE));

			mpd_song_free(song);
			mpd_response_finish(conn);
		}
	}

	draw_text(dc, song_name);
}
