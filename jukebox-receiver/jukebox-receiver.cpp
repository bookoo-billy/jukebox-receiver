// jukebox-receiver.cpp : Defines the entry point for the application.
//

#include "jukebox-receiver.h"

using namespace std;

using namespace boost::asio;
using namespace boost::asio::ip;

void* data;
PaStream* stream;

static int patestCallback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData)
{
	/* Cast data passed through stream to our structure. */
	float* out = (float*)outputBuffer;
	unsigned int i;
	(void)inputBuffer; /* Prevent unused variable warning. */

	for (i = 0; i < framesPerBuffer; i++)
	{
		*out++ = 1;
	}
	return 0;
}

void handle_message(jukebox::Message message) {
	if (message.messagetype() == jukebox::Message_MessageType::Message_MessageType_SONG_PREAMBLE) {
		PaError err = Pa_OpenDefaultStream(&stream,
			0,          /* no input channels */
			message.songpreamble().channels(),
			paFloat32,  /* 32 bit floating point output */
			message.songpreamble().samplerate(),
			256,        /* frames per buffer, i.e. the number
							   of sample frames that PortAudio will
							   request from the callback. Many apps
							   may want to use
							   paFramesPerBufferUnspecified, which
							   tells PortAudio to pick the best,
							   possibly changing, buffer size.*/
			patestCallback, /* this is your callback function */
			&data); /*This is a pointer that will be passed to
							   your callback*/

		if (err != paNoError) {
			cout << "Error while opening audio stream: " << Pa_GetErrorText(err) << endl;
		}

		err = Pa_StartStream(stream);

		if (err != paNoError) {
			cout << "Error while starting stream: " << Pa_GetErrorText(err) << endl;
		}
	}
	else if (message.messagetype() == jukebox::Message_MessageType::Message_MessageType_SONG_CHUNK) {

	}
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	PaError err = Pa_Initialize();
	if (err != paNoError) {
		cout << "Error while initialize PortAudio: " << Pa_GetErrorText(err) << endl;
	}

	io_service ioservice;
	tcp::endpoint tcp_endpoint{ tcp::v4(), 2187 };
	tcp::acceptor tcp_acceptor{ ioservice, tcp_endpoint };

	cout << "Awaiting connection from media server..." << endl;
	tcp::socket tcp_socket = tcp_acceptor.accept();
	cout << "Connected!" << endl;

	for (;;) {
		cout << "Reading message from socket.." << endl;

		jukebox::Message message;
		bool clean_eof = true;

		google::protobuf::io::FileInputStream istream(tcp_socket.native_handle());
		google::protobuf::util::ParseDelimitedFromZeroCopyStream(&message, &istream, &clean_eof);

		if (!clean_eof) {
			handle_message(message);
		}
		else {
			if (stream != NULL) {
				err = Pa_StopStream(stream);
				if (err != paNoError) {
					cout << "Error while stopping stream: " << Pa_GetErrorText(err) << endl;
				}
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	err = Pa_Terminate();
	if (err != paNoError) {
		cout << "Error while terminating PortAudio: " << Pa_GetErrorText(err) << endl;
	}

	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}