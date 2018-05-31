package main

import (
	"github.com/Shopify/sarama"
	tlog "github.com/thinkboy/log4go"
	"fmt"
	"time"
)

const ()

func main() {
	asyncSend()
}
func syncSend(){
	config := sarama.NewConfig()
	config.Producer.RequiredAcks = sarama.WaitForLocal
	config.Producer.Partitioner = sarama.NewHashPartitioner
	config.Producer.Return.Successes = true
	config.Producer.Return.Errors = true

	kafkaAddrs := []string{"kafka1:9092", "kafka2:9092"}
	tlog.Info("init kafka: %v", kafkaAddrs)
	ktopics := "webchat"
	producer, err := sarama.NewSyncProducer(kafkaAddrs, config)
	defer producer.Close()
	if nil != err {

	} else {
		idx :=1
		for {
			idx++
			sendmsg:= fmt.Sprintf("# %d this is a msg for testing!", idx)
			msg := &sarama.ProducerMessage{Topic: ktopics, Key: sarama.ByteEncoder("Job"),Value: sarama.ByteEncoder(sendmsg)}
			time.Sleep(time.Second* 2)
			producer.SendMessage(msg)

		}
	}
}

func asyncSend() {
	config := sarama.NewConfig()
	config.Producer.RequiredAcks = sarama.WaitForAll
	config.Producer.Partitioner = sarama.NewHashPartitioner
	config.Producer.Return.Successes = true
	config.Producer.Return.Errors = true

	kafkaAddrs := []string{"kafka1:9092", "kafka2:9092"}
	tlog.Info("init kafka: %v", kafkaAddrs)
	ktopics := "webchat"
	producer, err  := sarama.NewAsyncProducer(kafkaAddrs, config )
	defer producer.Close()
	if nil != err {

	} else {
		go func() {
			var (
				err *sarama.ProducerError
			)
			for {
				err = <-producer.Errors()
				if err != nil {
					tlog.Error("producer message error, partition:%d offset:%d key:%v value:%s error(%v)", err.Msg.Partition, err.Msg.Offset, err.Msg.Key, err.Msg.Value, err.Err)
				}
			}
		}()

		go func() {
			var (
				pm *sarama.ProducerMessage
			)
			for {
				pm = <-producer.Successes()
				if pm != nil {
					tlog.Info("producer message success, partition:%d offset:%d key:%v value:%s", pm.Partition, pm.Offset, pm.Key, pm.Value)
				}
			}
		}()
		idx :=1
		for {
			idx++
			sendmsg:= fmt.Sprintf("# %d this is a msg for testing!", idx)
			msg := &sarama.ProducerMessage{Topic: ktopics, Key: sarama.ByteEncoder("Job"),Value: sarama.ByteEncoder(sendmsg)}
			time.Sleep(time.Second* 2)
			producer.Input()<-msg

		}
	}
}
