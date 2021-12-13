package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"

	"github.com/tarm/serial"
)

func main() {
	port := flag.String("p", "/dev/ttyUSB0", "as Port Value of Serial")
	baud := flag.Int("b", 9600, "as BaudRate of Serial")
	rwType := flag.String("t", "read", "For Reading or Writing")
	flag.Parse()
	c := &serial.Config{Name: *port, Baud: *baud}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}
	switch *rwType {
	case "read":
		_, err := s.Write([]byte("value\n"))
		if err != nil {
			fmt.Println(err)
		}
		scanner := bufio.NewScanner(s)
		scanner.Scan()
		if scanner.Text() == "" {
			s.Close()
		}
		fmt.Println(scanner.Text())
	case "writeup":
		_, err := s.Write([]byte("type:entrance:direction:up\n"))
		if err != nil {
			log.Fatal(err)
		}
	case "writedown":
		_, err := s.Write([]byte("type:entrance:direction:down\n"))
		if err != nil {
			log.Fatal(err)
		}
	}
	s.Close()
}
