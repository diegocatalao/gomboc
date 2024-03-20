package environ

import (
	"os"
	"path"
	"path/filepath"

	"github.com/BurntSushi/toml"
	"github.com/rs/zerolog/log"
)

const CONF_FILE_NAME = "gomboc.toml"

type GombocConfig struct {
	GombocServer Server `toml:"server"`
	GombocClient Client `toml:"client"`
}

type Server struct {
	Host     string `toml:"host"`
	Port     int    `toml:"port"`
	Authtype string `toml:"authtype"`

	GombocServerAPI       ServerAPI       `toml:"api"`
	GombocServerDashboard ServerDashboard `toml:"dashboard"`
	GombocServerDatabase  ServerDatabase  `toml:"database"`
}

type Client struct {
	Web ClientWeb `toml:"web"`
	SSH ClientSSH `toml:"ssh"`
}

type ClientWeb struct {
	Host string `toml:"host"`
	Port int    `toml:"port"`
}

type ClientSSH struct {
	Host string `toml:"host"`
	Port int    `toml:"port"`
}

type ServerAPI struct {
	Host     string `toml:"host"`
	Port     int    `toml:"port"`
	Authtype string `toml:"authtype"`
}

type ServerDashboard struct {
	Host     string `toml:"host"`
	Port     int    `toml:"port"`
	Authtype string `toml:"authtype"`
}

type ServerDatabase struct {
	Host     string `toml:"host"`
	Port     int    `toml:"port"`
	Username string `toml:"username"`
	Password string `toml:"password"`
}

func GombocConfigLoader() GombocConfig {
	var config GombocConfig

	exec, _ := os.Executable()
	filepath := filepath.Join(path.Dir(exec), CONF_FILE_NAME)
	data, err := os.ReadFile(filepath)

	if err != nil {
		log.Error().Msgf("The file '%s' was not found. Skipped.", filepath)
		config = GombocConfig{}

		return config
	}

	_, err = toml.Decode(string(data), &config)

	if err != nil {
		panic("An error occurred when trying to parse the configuration file")
	}

	return config
}
