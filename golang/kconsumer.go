package main

import (
	"github.com/Shopify/sarama"
	"github.com/wvanbergen/kafka/consumergroup"
	"time"
	"os"
	"log"
	tlog "github.com/thinkboy/log4go"
	"fmt"
)

const (
	OFFSETS_PROCESSING_TIMEOUT_SECONDS = 10 * time.Second
	OFFSETS_COMMIT_INTERVAL = 10 * time.Second
)
func main() {
	kgrpcfg := consumergroup.NewConfig()
	kgrpcfg.Offsets.Initial = sarama.OffsetNewest
	kgrpcfg.Offsets.ProcessingTimeout = OFFSETS_PROCESSING_TIMEOUT_SECONDS
	sarama.Logger = log.New(os.Stdout, "[Sarama]", log.LstdFlags)
	kgrpcfg.Offsets.CommitInterval = OFFSETS_COMMIT_INTERVAL
	kgrpcfg.Zookeeper.Chroot = ""
	ktopics := [] string {"webchat","flytrack"}
	kzkaddrs := [] string {"zk1:2181",}
	cg,err := consumergroup.JoinConsumerGroup("SimGroup",ktopics,kzkaddrs,kgrpcfg)
	if nil!= err {
		tlog.Error("err: %v",err)
		fmt.Println(29)
	}else {
		go func() {
			for err := range cg.Errors() {
				tlog.Error("consumer error(%v)", err)
			}
			fmt.Println(35)
		}()

		for msg := range cg.Messages() {
			tlog.Info("deal with topic:%s, partitionId:%d, Offset:%d, Key:%s msg:%s", msg.Topic, msg.Partition, msg.Offset, msg.Key, msg.Value)
			cg.CommitUpto(msg)
		}
		fmt.Println(42)
	}

}
