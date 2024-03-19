package cmd

import (
	"gomboc/api"

	"github.com/spf13/cobra"
)

func init() {
	var host string  // The API local host address
	var port int     // The API local port
	var migrate bool // Sinalize automigrate for models

	var cmd = &cobra.Command{
		Use:   "api",
		Short: "Start Gomboc API",
		Long:  "Start the Gomboc API with a host (default 0.0.0.0) and a port (default 7020)",
		Run: func(cmd *cobra.Command, args []string) {
			// Start Gomboc API.
			//
			// Peer devices will register with the api. Control and management of devices will be
			// done through the administrator user interface.
			api.GombocAPI{Host: host, Port: port, AutoMigrate: migrate}.Initialize()
		},
	}

	cmd.Flags().StringVarP(&host, "host", "", "0.0.0.0", "API host address")
	cmd.Flags().IntVarP(&port, "port", "", 7020, "API port number")
	cmd.Flags().BoolVarP(&migrate, "migrate", "", false, "Auto migrate models")

	rootCmd.AddCommand(cmd)
}
