package main

import (
	"gomboc/cmd"
	"os"
	"time"

	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"
)

func setDefaultZeroLogger() {
	zerolog.SetGlobalLevel(zerolog.TraceLevel)
	zerolog.TimeFieldFormat = zerolog.TimeFormatUnix

	log.Logger = zerolog.New(zerolog.ConsoleWriter{Out: os.Stderr, TimeFormat: time.DateTime})
	log.Logger = log.Logger.With().Timestamp().Caller().Logger()
}

func main() {
	setDefaultZeroLogger() // set the global zloggger configuration

	cmd.Execute()
}
