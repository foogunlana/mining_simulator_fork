FROM alpine:3.6

RUN mkdir -p /home/bo/simulator

WORKDIR /home/bo/simulator

COPY . .

RUN make test && make all

CMD ['./test.o']
