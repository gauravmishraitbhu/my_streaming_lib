///*
// * Copyright (c) 2003 Fabrice Bellard
// *
// * Permission is hereby granted, free of charge, to any person obtaining a copy
// * of this software and associated documentation files (the "Software"), to deal
// * in the Software without restriction, including without limitation the rights
// * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// * copies of the Software, and to permit persons to whom the Software is
// * furnished to do so, subject to the following conditions:
// *
// * The above copyright notice and this permission notice shall be included in
// * all copies or substantial portions of the Software.
// *
// * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// * THE SOFTWARE.
// */
//
///**
// * @file
// * libavformat API example.
// *
// * Output a media file in any supported libavformat format. The default
// * codecs are used.
// * @example muxing.c
// */
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//
//#include <libavutil/avassert.h>
//#include <libavutil/channel_layout.h>
//#include <libavutil/opt.h>
//#include <libavutil/mathematics.h>
//#include <libavutil/timestamp.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libswresample/swresample.h>
//
//#define STREAM_DURATION   1000.0
//#define STREAM_FRAME_RATE 25 /* 25 images/s */
//#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
//
//#define SCALE_FLAGS SWS_BICUBIC
//
//// a wrapper around a single output AVStream
//typedef struct OutputStream {
//    AVStream *st;
//
//    /* pts of the next frame that will be generated */
//    int64_t next_pts;
//    int samples_count;
//
//    AVFrame *frame;
//    AVFrame *tmp_frame;
//
//    float t, tincr, tincr2;
//
//    struct SwsContext *sws_ctx;
//    struct SwrContext *swr_ctx;
//} OutputStream;
//
//static AVFormatContext *ifmt_ctx;
//static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt,int is_input)
//{
//    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
//    if(is_input){
//        printf("input--");
//    }else{
//        printf("output--");
//    }
//    printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
//           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
//           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
//           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
//           pkt->stream_index);
//}
//
//static int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
//{
//    /* rescale output packet timestamp values from codec to stream timebase */
//    av_packet_rescale_ts(pkt, *time_base, st->time_base);
//    pkt->stream_index = st->index;
//
//    /* Write the compressed frame to the media file. */
//    log_packet(fmt_ctx, pkt,0);
//    return av_interleaved_write_frame(fmt_ctx, pkt);
//}
//
///* Add an output stream. */
//static void add_stream(OutputStream *ost, AVFormatContext *oc,
//                       AVCodec **codec,
//                       enum AVCodecID codec_id)
//{
//    AVCodecContext *c;
//    int i;
//
//    /* find the encoder */
//    *codec = avcodec_find_encoder(codec_id);
//    if (!(*codec)) {
//        fprintf(stderr, "Could not find encoder for '%s'\n",
//                avcodec_get_name(codec_id));
//        exit(1);
//    }
//
//    ost->st = avformat_new_stream(oc, *codec);
//    if (!ost->st) {
//        fprintf(stderr, "Could not allocate stream\n");
//        exit(1);
//    }
//    ost->st->id = oc->nb_streams-1;
//    c = ost->st->codec;
////    avcodec_copy_context(c, ifmt_ctx->streams[0]->codec);
//    switch ((*codec)->type) {
//        case AVMEDIA_TYPE_AUDIO:
//            c->sample_fmt  = (*codec)->sample_fmts ?
//            (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
//            c->bit_rate    = 64000;
//            c->sample_rate = 44100;
//            if ((*codec)->supported_samplerates) {
//                c->sample_rate = (*codec)->supported_samplerates[0];
//                for (i = 0; (*codec)->supported_samplerates[i]; i++) {
//                    if ((*codec)->supported_samplerates[i] == 44100)
//                        c->sample_rate = 44100;
//                }
//            }
//            c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
//            c->channel_layout = AV_CH_LAYOUT_STEREO;
//            if ((*codec)->channel_layouts) {
//                c->channel_layout = (*codec)->channel_layouts[0];
//                for (i = 0; (*codec)->channel_layouts[i]; i++) {
//                    if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
//                        c->channel_layout = AV_CH_LAYOUT_STEREO;
//                }
//            }
//            c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
//            ost->st->time_base = (AVRational){ 1, c->sample_rate };
//            break;
//
//        case AVMEDIA_TYPE_VIDEO:
//            c->codec_id = codec_id;
//
//            c->bit_rate = 400000;
//            /* Resolution must be a multiple of two. */
//            c->width    = 352;
//            c->height   = 288;
//            /* timebase: This is the fundamental unit of time (in seconds) in terms
//             * of which frame timestamps are represented. For fixed-fps content,
//             * timebase should be 1/framerate and timestamp increments should be
//             * identical to 1. */
//            ost->st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
//            c->time_base       = ost->st->time_base;
//
//           // c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
//            c->pix_fmt       = STREAM_PIX_FMT;
//            if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
//                /* just for testing, we also add B frames */
//                c->max_b_frames = 2;
//            }
//            if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
//                /* Needed to avoid using macroblocks in which some coeffs overflow.
//                 * This does not happen with normal video, it just happens here as
//                 * the motion of the chroma plane does not match the luma plane. */
//                c->mb_decision = 2;
//            }
//            break;
//
//        default:
//            break;
//    }
//
//    /* Some formats want stream headers to be separate. */
//    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
//        c->flags |= CODEC_FLAG_GLOBAL_HEADER;
//}
//
///**************************************************************/
///* audio output */
//
//static AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
//                                  uint64_t channel_layout,
//                                  int sample_rate, int nb_samples)
//{
//    AVFrame *frame = av_frame_alloc();
//    int ret;
//
//    if (!frame) {
//        fprintf(stderr, "Error allocating an audio frame\n");
//        exit(1);
//    }
//
//    frame->format = sample_fmt;
//    frame->channel_layout = channel_layout;
//    frame->sample_rate = sample_rate;
//    frame->nb_samples = nb_samples;
//
//    if (nb_samples) {
//        ret = av_frame_get_buffer(frame, 0);
//        if (ret < 0) {
//            fprintf(stderr, "Error allocating an audio buffer\n");
//            exit(1);
//        }
//    }
//
//    return frame;
//}
//
//static void open_audio(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
//{
//    AVCodecContext *c;
//    int nb_samples;
//    int ret;
//    AVDictionary *opt = NULL;
//
//    c = ost->st->codec;
//
//    /* open it */
//    av_dict_copy(&opt, opt_arg, 0);
//    ret = avcodec_open2(c, codec, &opt);
//    av_dict_free(&opt);
//    if (ret < 0) {
//        fprintf(stderr, "Could not open audio codec: %s\n", av_err2str(ret));
//        exit(1);
//    }
//
//    /* init signal generator */
//    ost->t     = 0;
//    ost->tincr = 2 * M_PI * 110.0 / c->sample_rate;
//    /* increment frequency by 110 Hz per second */
//    ost->tincr2 = 2 * M_PI * 110.0 / c->sample_rate / c->sample_rate;
//
//    if (c->codec->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE)
//        nb_samples = 10000;
//    else
//        nb_samples = c->frame_size;
//
//    ost->frame     = alloc_audio_frame(c->sample_fmt, c->channel_layout,
//                                       c->sample_rate, nb_samples);
//    ost->tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_S16, c->channel_layout,
//                                       c->sample_rate, nb_samples);
//
//    /* create resampler context */
//    ost->swr_ctx = swr_alloc();
//    if (!ost->swr_ctx) {
//        fprintf(stderr, "Could not allocate resampler context\n");
//        exit(1);
//    }
//
//    /* set options */
//    av_opt_set_int       (ost->swr_ctx, "in_channel_count",   c->channels,       0);
//    av_opt_set_int       (ost->swr_ctx, "in_sample_rate",     c->sample_rate,    0);
//    av_opt_set_sample_fmt(ost->swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
//    av_opt_set_int       (ost->swr_ctx, "out_channel_count",  c->channels,       0);
//    av_opt_set_int       (ost->swr_ctx, "out_sample_rate",    c->sample_rate,    0);
//    av_opt_set_sample_fmt(ost->swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);
//
//    /* initialize the resampling context */
//    if ((ret = swr_init(ost->swr_ctx)) < 0) {
//        fprintf(stderr, "Failed to initialize the resampling context\n");
//        exit(1);
//    }
//}
//
///* Prepare a 16 bit dummy audio frame of 'frame_size' samples and
// * 'nb_channels' channels. */
//static AVFrame *get_audio_frame(OutputStream *ost)
//{
//    AVFrame *frame = ost->tmp_frame;
//    int j, i, v;
//    int16_t *q = (int16_t*)frame->data[0];
//
//    /* check if we want to generate more frames */
//    if (av_compare_ts(ost->next_pts, ost->st->codec->time_base,
//                      STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
//        return NULL;
//
//    for (j = 0; j <frame->nb_samples; j++) {
//        v = (int)(sin(ost->t) * 10000);
//        for (i = 0; i < ost->st->codec->channels; i++)
//            *q++ = v;
//        ost->t     += ost->tincr;
//        ost->tincr += ost->tincr2;
//    }
//
//    frame->pts = ost->next_pts;
//    ost->next_pts  += frame->nb_samples;
//
//    return frame;
//}
//
///*
// * encode one audio frame and send it to the muxer
// * return 1 when encoding is finished, 0 otherwise
// */
//static int write_audio_frame(AVFormatContext *oc, OutputStream *ost)
//{
//    AVCodecContext *c;
//    AVPacket pkt = { 0 }; // data and size must be 0;
//    AVFrame *frame;
//    int ret;
//    int got_packet;
//    int dst_nb_samples;
//
//    av_init_packet(&pkt);
//    c = ost->st->codec;
//
//    frame = get_audio_frame(ost);
//
//    if (frame) {
//        /* convert samples from native format to destination codec format, using the resampler */
//        /* compute destination number of samples */
//        dst_nb_samples = av_rescale_rnd(swr_get_delay(ost->swr_ctx, c->sample_rate) + frame->nb_samples,
//                                        c->sample_rate, c->sample_rate, AV_ROUND_UP);
//        av_assert0(dst_nb_samples == frame->nb_samples);
//
//        /* when we pass a frame to the encoder, it may keep a reference to it
//         * internally;
//         * make sure we do not overwrite it here
//         */
//        ret = av_frame_make_writable(ost->frame);
//        if (ret < 0)
//            exit(1);
//
//        /* convert to destination format */
//        ret = swr_convert(ost->swr_ctx,
//                          ost->frame->data, dst_nb_samples,
//                          (const uint8_t **)frame->data, frame->nb_samples);
//        if (ret < 0) {
//            fprintf(stderr, "Error while converting\n");
//            exit(1);
//        }
//        frame = ost->frame;
//
//        frame->pts = av_rescale_q(ost->samples_count, (AVRational){1, c->sample_rate}, c->time_base);
//        ost->samples_count += dst_nb_samples;
//    }
//
//    ret = avcodec_encode_audio2(c, &pkt, frame, &got_packet);
//    if (ret < 0) {
//        fprintf(stderr, "Error encoding audio frame: %s\n", av_err2str(ret));
//        exit(1);
//    }
//
//    if (got_packet) {
//        ret = write_frame(oc, &c->time_base, ost->st, &pkt);
//        if (ret < 0) {
//            fprintf(stderr, "Error while writing audio frame: %s\n",
//                    av_err2str(ret));
//            exit(1);
//        }
//    }
//
//    return (frame || got_packet) ? 0 : 1;
//}
//
///**************************************************************/
///* video output */
//
//static AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
//{
//    AVFrame *picture;
//    int ret;
//
//    picture = av_frame_alloc();
//    if (!picture)
//        return NULL;
//
//    picture->format = pix_fmt;
//    picture->width  = width;
//    picture->height = height;
//
//    /* allocate the buffers for the frame data */
//    ret = av_frame_get_buffer(picture, 32);
//    if (ret < 0) {
//        fprintf(stderr, "Could not allocate frame data.\n");
//        exit(1);
//    }
//
//    return picture;
//}
//
//static void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
//{
//    int ret;
//    AVCodecContext *c = ost->st->codec;
//    AVDictionary *opt = NULL;
//
//    av_dict_copy(&opt, opt_arg, 0);
//    av_opt_set(c->priv_data, "preset", "slow", 0);
//    
//    /* open the codec */
//    ret = avcodec_open2(c, codec, &opt);
//    av_dict_free(&opt);
//    if (ret < 0) {
//        fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
//        exit(1);
//    }
//
//    /* allocate and init a re-usable frame */
//    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
//    if (!ost->frame) {
//        fprintf(stderr, "Could not allocate video frame\n");
//        exit(1);
//    }
//
//    /* If the output format is not YUV420P, then a temporary YUV420P
//     * picture is needed too. It is then converted to the required
//     * output format. */
//    ost->tmp_frame = NULL;
//    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
//        ost->tmp_frame = alloc_picture(AV_PIX_FMT_YUV420P, c->width, c->height);
//        if (!ost->tmp_frame) {
//            fprintf(stderr, "Could not allocate temporary picture\n");
//            exit(1);
//        }
//    }
//}
//
///* Prepare a dummy image. */
//static void fill_yuv_image(AVFrame *pict, int frame_index,
//                           int width, int height)
//{
//    int x, y, i, ret;
//
//    /* when we pass a frame to the encoder, it may keep a reference to it
//     * internally;
//     * make sure we do not overwrite it here
//     */
//    ret = av_frame_make_writable(pict);
//    if (ret < 0)
//        exit(1);
//
//    i = frame_index;
//
//    /* Y */
//    for (y = 0; y < height; y++)
//        for (x = 0; x < width; x++)
//            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
//
//    /* Cb and Cr */
//    for (y = 0; y < height / 2; y++) {
//        for (x = 0; x < width / 2; x++) {
//            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
//            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
//        }
//    }
//}
//
//static AVFrame *get_video_frame(OutputStream *ost)
//{
//    AVCodecContext *c = ost->st->codec;
//
//    /* check if we want to generate more frames */
//    if (av_compare_ts(ost->next_pts, ost->st->codec->time_base,
//                      STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
//        return NULL;
//
//    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
//        /* as we only generate a YUV420P picture, we must convert it
//         * to the codec pixel format if needed */
//        if (!ost->sws_ctx) {
//            ost->sws_ctx = sws_getContext(c->width, c->height,
//                                          AV_PIX_FMT_YUV420P,
//                                          c->width, c->height,
//                                          c->pix_fmt,
//                                          SCALE_FLAGS, NULL, NULL, NULL);
//            if (!ost->sws_ctx) {
//                fprintf(stderr,
//                        "Could not initialize the conversion context\n");
//                exit(1);
//            }
//        }
//        fill_yuv_image(ost->tmp_frame, ost->next_pts, c->width, c->height);
//        sws_scale(ost->sws_ctx,
//                  (const uint8_t * const *)ost->tmp_frame->data, ost->tmp_frame->linesize,
//                  0, c->height, ost->frame->data, ost->frame->linesize);
//    } else {
//        fill_yuv_image(ost->frame, ost->next_pts, c->width, c->height);
//    }
//
//    ost->frame->pts = ost->next_pts++;
//
//    return ost->frame;
//}
//
//static int write_video_frame2(AVFormatContext *oc, OutputStream *ost,AVFrame *frame){
//    int ret;
//    AVCodecContext *c;
//
//    int got_packet = 0;
//    
//    c = ost->st->codec;
//    
//    AVPacket pkt = { 0 };
//    av_init_packet(&pkt);
//    
//    /* encode the image */
//    ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
//    if (ret < 0) {
//        fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
//        exit(1);
//    }
//    
//    if (got_packet) {
//        ret = write_frame(oc, &ifmt_ctx->streams[0]->time_base, ost->st, &pkt);
//    } else {
//        ret = 0;
//    }
//    
//    if (ret < 0) {
//        fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
//        exit(1);
//    }
//    
//    return (frame || got_packet) ? 0 : 1;
//
//}
//
///*
// * encode one video frame and send it to the muxer
// * return 1 when encoding is finished, 0 otherwise
// */
//static int write_video_frame(AVFormatContext *oc, OutputStream *ost)
//{
//    int ret;
//    AVCodecContext *c;
//    AVFrame *frame;
//    int got_packet = 0;
//
//    c = ost->st->codec;
//
//    frame = get_video_frame(ost);
//
//    if (oc->oformat->flags & AVFMT_RAWPICTURE) {
//        /* a hack to avoid data copy with some raw video muxers */
//        AVPacket pkt;
//        av_init_packet(&pkt);
//
//        if (!frame)
//            return 1;
//
//        pkt.flags        |= AV_PKT_FLAG_KEY;
//        pkt.stream_index  = ost->st->index;
//        pkt.data          = (uint8_t *)frame;
//        pkt.size          = sizeof(AVPicture);
//
//        pkt.pts = pkt.dts = frame->pts;
//        av_packet_rescale_ts(&pkt, c->time_base, ost->st->time_base);
//
//        ret = av_interleaved_write_frame(oc, &pkt);
//    } else {
//        AVPacket pkt = { 0 };
//        av_init_packet(&pkt);
//
//        /* encode the image */
//        ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
//        if (ret < 0) {
//            fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
//            exit(1);
//        }
//
//        if (got_packet) {
//            ret = write_frame(oc, &c->time_base, ost->st, &pkt);
//        } else {
//            ret = 0;
//        }
//    }
//
//    if (ret < 0) {
//        fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
//        exit(1);
//    }
//
//    return (frame || got_packet) ? 0 : 1;
//}
//
//static void close_stream(AVFormatContext *oc, OutputStream *ost)
//{
//    avcodec_close(ost->st->codec);
//    av_frame_free(&ost->frame);
//    av_frame_free(&ost->tmp_frame);
//    sws_freeContext(ost->sws_ctx);
//    swr_free(&ost->swr_ctx);
//}
//
//static int open_input_file(char *file_name){
//   // File *f = NULL:
//    int ret=-1;
//    ifmt_ctx = NULL;
//    int i;
//    ret = avformat_open_input(&ifmt_ctx, file_name, NULL, NULL);
//    if(ret<0){
//        printf("could not open input file");
//        return ret;
//    }
//    
//    ret = avformat_find_stream_info(ifmt_ctx, NULL);
//    if(ret < 0){
//        printf("could not find stream info");
//        return ret;
//    }
//    
//    for(i=0;i<(ifmt_ctx)->nb_streams;i++){
//        AVStream *input_stream = (ifmt_ctx)->streams[i];
//        if(input_stream->codec->codec_type != AVMEDIA_TYPE_VIDEO){
//            continue;
//        }
//        
//        AVCodecContext *codec_ctx = input_stream->codec;
//        ret = avcodec_open2(codec_ctx, avcodec_find_decoder(codec_ctx->codec_id), NULL);
//        if(ret < 0){
//            printf("cannot open decoder\n");
//        }
//    }
//    
//   // f = fopen(file_name, "rb");
////    if (!f) {
////        fprintf(stderr, "Could not open %s\n", filename);
////        exit(1);
////    }
//    av_dump_format(ifmt_ctx, 0, file_name, 0);
//    return 0;
//}
//
///**************************************************************/
///* media file output */
//
//int main(int argc, char **argv)
//{
//    OutputStream video_st = { 0 }, audio_st = { 0 };
//    const char *filename;
//    const char *input_filename;
//    AVPacket packet = { .data = NULL, .size = 0 };
//    AVOutputFormat *fmt;
//    AVFormatContext *oc;
//
//    AVCodec *audio_codec, *video_codec;
//    int ret;
//    int have_video = 0, have_audio = 0;
//    int encode_video = 0, encode_audio = 0;
//    AVDictionary *opt = NULL;
//    AVFrame *frame = NULL;
//    int got_frame = 0;
//
//    /* Initialize libavcodec, and register all codecs and formats. */
//    av_register_all();
//    avformat_network_init();
//    
//    filename = "rtmp://localhost:1935/live/myStream";
//    input_filename = "rtmp://localhost:1935/vod/sample.mp4";
//    
//    open_input_file(input_filename);
//    AVOutputFormat *dummy = av_guess_format(NULL, filename, "video/x-flv");
//    dummy->video_codec = AV_CODEC_ID_H264;
//
//    /* allocate the output media context */
//    avformat_alloc_output_context2(&oc, NULL, "FLV", filename);
//  
//
//    av_log_set_level(AV_LOG_INFO);
//    fmt = oc->oformat;
//
//    /* Add the audio and video streams using the default format codecs
//     * and initialize the codecs. */
//    if (fmt->video_codec != AV_CODEC_ID_NONE) {
//        add_stream(&video_st, oc, &video_codec, fmt->video_codec);
//        have_video = 1;
//        encode_video = 1;
//    }
////    if (fmt->audio_codec != AV_CODEC_ID_NONE) {
////        add_stream(&audio_st, oc, &audio_codec, fmt->audio_codec);
////        have_audio = 1;
////        encode_audio = 1;
////    }
//
//    /* Now that all the parameters are set, we can open the audio and
//     * video codecs and allocate the necessary encode buffers. */
//    if (have_video)
//        open_video(oc, video_codec, &video_st, opt);
//
//    if (have_audio)
//        open_audio(oc, audio_codec, &audio_st, opt);
//
//    av_dump_format(oc, 0, filename, 1);
//
//    /* open the output file, if needed */
//    if (!(fmt->flags & AVFMT_NOFILE)) {
//        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
//        if (ret < 0) {
//            fprintf(stderr, "Could not open '%s': %s\n", filename,
//                    av_err2str(ret));
//            return 1;
//        }
//    }
//
//    /* Write the stream header, if any. */
//    ret = avformat_write_header(oc, &opt);
//    if (ret < 0) {
//        fprintf(stderr, "Error occurred when opening output file: %s\n",
//                av_err2str(ret));
//        return 1;
//    }
//
//  
//    while(1){
//        ret = av_read_frame(ifmt_ctx, &packet);
//        if(ret < 0){
//            break;
//        }
//        
//
//        AVStream *in_stream = ifmt_ctx->streams[packet.stream_index];
//        
//        if(in_stream->codec->codec_type != AVMEDIA_TYPE_VIDEO){
//            continue;
//        }
//
//
//        
//        frame = av_frame_alloc();
//        ret = avcodec_decode_video2(in_stream->codec, frame, &got_frame, &packet);
//        log_packet(ifmt_ctx, &packet,1);
//        if(ret<0){
//            av_frame_free(&frame);
//            printf("could not decode a packet....");
//            break;
//        }
//        
//        if(got_frame){
//            frame->pts = av_frame_get_best_effort_timestamp(frame);
//            
//            write_video_frame2(oc, &video_st,frame);
//        }
//        
//    }
//    
//
//    /* Write the trailer, if any. The trailer must be written before you
//     * close the CodecContexts open when you wrote the header; otherwise
//     * av_write_trailer() may try to use memory that was freed on
//     * av_codec_close(). */
//    av_write_trailer(oc);
//
//    /* Close each codec. */
//    if (have_video)
//        close_stream(oc, &video_st);
//    if (have_audio)
//        close_stream(oc, &audio_st);
//
//    if (!(fmt->flags & AVFMT_NOFILE))
//    /* Close the output file. */
//        avio_close(oc->pb);
//
//    /* free the stream */
//    avformat_free_context(oc);
//    int i;
//    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
//        avcodec_close(ifmt_ctx->streams[i]->codec);
//        
//    }
//
//    avformat_close_input(&ifmt_ctx);
//
//    return 0;
//}
