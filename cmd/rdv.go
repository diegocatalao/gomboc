package cmd

import (
	"github.com/spf13/cobra"
)

func init() {
	var host string // The Rendezvous server address
	var port int    // The Rendezvous server port

	var cmd = &cobra.Command{
		Use:   "rdv",
		Short: "Start Gomboc RDV service (rendezvous server)",
		Long:  "Start Gomboc RDV service with a host (default 0.0.0.0) and a port (default 7021)",
		Run: func(cmd *cobra.Command, args []string) {
			panic("The command line for 'rdv' not implemented yet!")
		},
	}

	cmd.Flags().StringVarP(&host, "host", "", "", "RDV host address")
	cmd.Flags().IntVarP(&port, "port", "", 7020, "RDV port number")

	rootCmd.AddCommand(cmd)
}
