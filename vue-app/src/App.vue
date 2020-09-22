<template>
    <v-app>
        <v-navigation-drawer v-model="drawer" :clipped="$vuetify.breakpoint.lgAndUp" app>
            <v-list dense nav class="py-0">
                <v-list-item two-line :class="miniVariant && 'px-0'">
                    <v-list-item-avatar>
                        <img :src="require('./assets/logo.png')" />
                    </v-list-item-avatar>

                    <v-list-item-content>
                        <v-list-item-title>BBQ Controller</v-list-item-title>
                        <v-list-item-subtitle>Controls BBQ</v-list-item-subtitle>
                    </v-list-item-content>
                </v-list-item>
                <v-divider></v-divider>

                <v-list-item link to="/">
                    <v-list-item-icon>
                        <v-icon>mdi-home</v-icon>
                    </v-list-item-icon>

                    <v-list-item-content>
                        <v-list-item-title>Home</v-list-item-title>
                    </v-list-item-content>
                </v-list-item>

                <v-list-item link to="/about">
                    <v-list-item-icon>
                        <v-icon>mdi-help-circle</v-icon>
                    </v-list-item-icon>

                    <v-list-item-content>
                        <v-list-item-title>About Us</v-list-item-title>
                    </v-list-item-content>
                </v-list-item>

            </v-list>
        </v-navigation-drawer>

        <v-app-bar :clipped-left="$vuetify.breakpoint.lgAndUp" app color="#215027" dark>
            <v-app-bar-nav-icon @click.stop="drawer = !drawer" />
            <div class="d-flex align-center">
                <v-img
                    alt="Vuetify Logo"
                    class="shrink mr-2"
                    contain
                    :src="require('./assets/logo.png')"
                    transition="scale-transition"
                    width="40"
                />BBQ Controller
            </div>

            <v-spacer></v-spacer>

            
        </v-app-bar>

        <v-main>
            <router-view></router-view>
        </v-main>

        <v-footer app>
            <span>&copy; 2020</span>
        </v-footer>

    </v-app>
</template>
<script>
export default {
    name: "App",

    components: {  },

    data: () => ({
        drawer: null,
        miniVariant: false,
        showCart: false,
        snackbar: false,
        links: [
            {
                text: "About Us",
                href: "/about"
            },
            {
                text: "Become a Sponsor",
                href: "/becomeASponsor"
            },
            {
                text: "2021 Sponsors",
                href: "/2021Sponsors"
            }
        ]
    }),
}
</script>
<style lang="scss">
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

#nav {
  padding: 30px;

  a {
    font-weight: bold;
    color: #2c3e50;

    &.router-link-exact-active {
      color: #42b983;
    }
  }
}
</style>
