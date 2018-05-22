package main

import (
	"fmt"

	"os"
	"os/signal"
	"syscall"

	"github.com/Shopify/sarama"
	log "github.com/thinkboy/log4go"
)

var consumer sarama.Consumer

func main() {
	ctopic := "infoqueue"
	consumeMsg(ctopic)
	InitSignal()
	defer consumer.Close()
}

func consumeMsg(ctopic string) {
	config := sarama.NewConfig()
	//接收失败通知
	config.Consumer.Return.Errors = true
	//设置使用的kafka版本,如果低于V0_10_0_0版本,消息中的timestrap没有作用.需要消费和生产同时配置
	config.Version = sarama.V0_10_0_0
	//新建一个消费者
	var e error
	consumer, e = sarama.NewConsumer([]string{"kafka:9092"}, config)
	if e != nil {
		panic("error get consumer")
	}

	//根据消费者获取指定的主题分区的消费者,Offset这里指定为获取最新的消息.
	parts, _ := consumer.Partitions(ctopic)
	for _, i := range parts {
		pc, _ := consumer.ConsumePartition(ctopic, i, sarama.OffsetNewest)

		go getPartitionMsg(&pc)
	}
}

func getPartitionMsg(p *sarama.PartitionConsumer) {
	defer (*p).Close()
	for {
		select {
		//接收消息通道和错误通道的内容.
		case msg := <-(*p).Messages():
			fmt.Println("msg offset: ", msg.Offset, " partition: ", msg.Partition, " timestrap: ", msg.Timestamp.Format("2006-Jan-02 15:04"), " value: ", string(msg.Value))
		case err := <-(*p).Errors():
			fmt.Println(err.Err)
		}
	}

}

func InitSignal() {
	c := make(chan os.Signal, 1)
	signal.Notify(c, syscall.SIGHUP, syscall.SIGQUIT, syscall.SIGTERM, syscall.SIGINT, syscall.SIGSTOP)
	for {
		s := <-c
		log.Info("router[%s] get a signal %s", "1.0.0", s.String())
		switch s {
		case syscall.SIGQUIT, syscall.SIGTERM, syscall.SIGSTOP, syscall.SIGINT:
			return
		default:
			return
		}
	}
}
