package main

import (
	"github.com/Shopify/sarama"
	"time"
	"qiniupkg.com/x/log.v7"
	"fmt"
	"os"
	"os/signal"
	"sync"
)

var Address = []string{"kafka:9092",}

func main()  {
   // syncProducer(Address)
    asyncProducer1(Address)
}

func syncProducer(addr [] string){
	cfg := sarama.NewConfig()
	cfg.Producer.Return.Successes = true
	cfg.Producer.Timeout = 5 * time.Second
	p,err := sarama.NewSyncProducer(Address,cfg)
	if nil != err {
		log.Printf("sarama.NewSyncProducer err, message %s\n", err.Error())
		return
	}
	defer p.Close()
	topic := "infoqueue"
	srcValue := "sync: this is a message, index = %d"
	for i:=0; i< 10; i++ {
		value:= fmt.Sprintf(srcValue, i)
		msg := &sarama.ProducerMessage{
			Topic:topic,
			Value:sarama.ByteEncoder(value),
		}
		part , offset ,err := p.SendMessage(msg)
		if err != nil {
			log.Printf("send message(%s) err=%s \n", value, err)
		}else {
			fmt.Fprintf(os.Stdout, value + " Successfully Send + ，partition=%d, offset=%d \n", part, offset)
		}
	}
}

func asyncProducer1(address []string)  {
	config := sarama.NewConfig()
	config.Producer.Return.Successes = true
	//config.Producer.Partitioner = 默认为message的hash
	p, err := sarama.NewAsyncProducer(address, config)
	if err != nil {
		log.Printf("sarama.NewSyncProducer err, message=%s \n", err)
		return
	}

	//Trap SIGINT to trigger a graceful shutdown.
	signals := make(chan os.Signal, 1)
	signal.Notify(signals, os.Interrupt)

	var wg sync.WaitGroup
	var enqueued, successes, errors int
	wg.Add(2) //2 goroutine

	// 发送成功message计数
	go func() {
		defer wg.Done()
		for range p.Successes() {
			successes++
		}
	}()

	// 发送失败计数
	go func() {
		defer wg.Done()
		for err := range p.Errors() {
			log.Printf("%s 发送失败，err：%s\n", err.Msg, err.Err)
			errors++
		}
	}()

	// 循环发送信息
	asrcValue := "async-goroutine: this is a message. index=%d"
	var i int
LoopP1:
	for {
		i++
		value := fmt.Sprintf(asrcValue, i)
		msg := &sarama.ProducerMessage{
			Topic:"infoqueue",
			Value:sarama.ByteEncoder(value),
		}
		select {
		case p.Input() <- msg: // 发送消息
			enqueued++
			fmt.Fprintln(os.Stdout, value)
		case <-signals: // 中断信号
			p.AsyncClose()
			break LoopP1
		}
		time.Sleep(2 * time.Second)
	}
	wg.Wait()

	fmt.Fprintf(os.Stdout, "发送数=%d，发送成功数=%d，发送失败数=%d \n", enqueued, successes, errors)

}

func asyncProducer2(address []string)  {
	config := sarama.NewConfig()
	config.Producer.Return.Errors = true
	p, err := sarama.NewAsyncProducer(address, config)
	if err != nil {
		log.Printf("sarama.NewSyncProducer err, message=%s \n", err)
		return
	}

	//Trap SIGINT to trigger a graceful shutdown.
	signals := make(chan os.Signal, 1)
	signal.Notify(signals, os.Interrupt)

	var enqueued, successes, errors int
	asrcValue := "async-select: this is a message. index=%d"
	var i int
LoopP2:
	for {
		i++
		value := fmt.Sprintf(asrcValue, i)
		msg := &sarama.ProducerMessage{
			Topic:"infoqueue",
			Value:sarama.ByteEncoder(value),
		}
		select {
		case p.Input() <- msg:
			fmt.Fprintln(os.Stdout, value)
			enqueued++
		case <-p.Successes():
			successes++
		case err := <-p.Errors():
			log.Printf("%s 发送失败，err：%s\n", err.Msg, err.Err)
			errors++
		case <-signals:
			p.AsyncClose()
			break LoopP2
		}
		time.Sleep(2 * time.Second)
	}

	fmt.Fprintf(os.Stdout, "发送数=%d，发送失败数=%d \n", enqueued, errors)
}
