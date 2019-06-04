# an implementation of a sample block ring buffer

Problem:
- we are sampling sample wise from a sensor etc.
- we want to process the samples as block

Soultion:
- a ringbuffer which takes single samples as input (write operation)
- a ringbuffer that provides the collected samples as block (read operation)

A note on the implementation:
- super simple straight foreward implementation
- we are not checking if the write pointer/write block, means the read blocks are silently overwriten.
- this implementation only works, if the consumer(read) is quicker than the producer
- we are using const variables. That's why we to provide an variable declaration macro

Testing:
- use `build.sh` to build and run `block_rb` after
- use `testValgrind.sh` to build and run valgrind after
- the test simulates a ADS1292 EEG sensor. This sensor samples 24Bit values. In this test we want to use 255Byte frames to send the samples as block. We need 4 Bytes for message header. So we have 251Bytes left for a sample frame. In total we can transmit 83 samples per frame. This test writes 4GSamples though a ringbuffer, that can store 8 blocks of 83 samples. Each sample gets a unique id(=number) when put into the buffer. When reading a block of samples, this uuid is checked. in this case the test would fail.
