package cmd

import (
	"github.com/spf13/cobra"
)

func init() {
	var host string // The Rendezvous server address
	var port int    // The Rendezvous server port

	var cmd = &cobra.Command{
		Use:   "node",
		Short: "Start Gomboc Node service (the client service)",
		Long:  "Start Gomboc Node service with a RDV hub host (rdv-ip) and a port (rdv-port 7021)",
		Run: func(cmd *cobra.Command, args []string) {
			if host == "" {
				panic("Rendezvous address cannot to be empty!")
			}

			panic("The command line for 'node' not implemented yet!")
		},
	}

	cmd.Flags().StringVarP(&host, "host", "", "", "Rendezvous host address")
	cmd.Flags().IntVarP(&port, "port", "", 7020, "Rendezvous port number")

	rootCmd.AddCommand(cmd)
}
