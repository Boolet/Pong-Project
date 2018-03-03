By:
Avi Miller 64299497
David Wong 22604337
Matthew Sparks 42602434
Andy Le 20681445

Server - Known to run only on MacOS X, on Xcode. It may work in other environments, but they are untested. Runs on port 8888.

Client - Hold up or down arrow key to move the paddle. Input may not be captured if the key is tapped briefly.

To change latency settings, go to CommunicationProtocol.hpp and in the MessageHandler, change the setMode to either FIXED_LATENCY, RANDOM_LATENCY, or INCREMENTAL_LATENCY